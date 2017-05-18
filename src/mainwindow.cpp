#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace HELPER
{
    TagItem *createExampleTagTree()
    {
        TagItem *root = new TagItem("Root0");
        TagItem *item1 = new TagItem("Root", root);
        TagItem *item1_1 = new TagItem("A", item1);
        TagItem *item1_2 = new TagItem("B", item1);
        return root;
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    lastUsedFileName(QDir::current().dirName())
{
    ui->setupUi(this);
    setWindowTitle(QMAKE_TARGET_PRODUCT);

    TagTreeModel *model = new TagTreeModel(this);
    model->setColorHelper(&color_helper);
    model->setTagChecker(&tag_checker);
    connect(model, &TagTreeModel::invalidValueSetted, this, &MainWindow::invalidValueSettedByUserToTreeViewModel);
    TagItem *root = HELPER::createExampleTagTree();
    model->setRoot(root);
    ui->tagsTreeView->setModel(model);

    ui->tagsTreeView->setDragEnabled(true);
    ui->tagsTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tagsTreeView->setAcceptDrops(true);
    ui->tagsTreeView->setDropIndicatorShown(true);
    ui->tagsTreeView->setDragDropMode(QAbstractItemView::InternalMove);

    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::on_about_triggered);
    connect(ui->actionAboutQt, &QAction::triggered, this, &MainWindow::on_about_qt_triggered);
    connect(ui->actionSetFont, &QAction::triggered, this, &MainWindow::on_set_font_triggered);
    connect(ui->tagsTreeView, &QTreeView::customContextMenuRequested, this, &MainWindow::customContextMenuRequestedForTreeView);
    connect(ui->pushButton_AddNew, &QPushButton::clicked, this, &MainWindow::on_treeview_new_triggered);
    connect(ui->pushButton_AddNewToRoot, &QPushButton::clicked, this, &MainWindow::on_treeview_new_to_root_triggered);
    connect(ui->pushButton_Remove, &QPushButton::clicked, this, &MainWindow::on_treeview_remove_triggered);
    connect(ui->pushButton_Up, &QPushButton::clicked, this, &MainWindow::on_treeview_up_triggered);
    connect(ui->pushButton_Down, &QPushButton::clicked, this, &MainWindow::on_treeview_down_triggered);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::on_load_file_clicked);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::on_save_file_clicked);
    loadSettings();
    QFile file(lastUsedFileName);
    if(file.exists() && QFileInfo(file).isFile())
        loadFile(lastUsedFileName);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    const int result = QMessageBox::question(this, qApp->applicationName(),
                                             tr("Do you want to exit and save settings?"),
                                             QMessageBox::Ok|QMessageBox::Cancel);
    if(result == QMessageBox::Ok) {
       storeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::loadFile(const QString& filename)
{
    TagStoreHelperXML helper(filename);
    TagItem *root = helper.load();
    if(helper.hasError()) {
        QMessageBox::warning(this, tr("Attention"), helper.errorString());
    }
    if(root != nullptr) {
        TagTreeModel *newModel = new TagTreeModel(this);
        newModel->setColorHelper(&color_helper);
        newModel->setTagChecker(&tag_checker);
        newModel->setRoot(root);
        connect(newModel, &TagTreeModel::invalidValueSetted, this, &MainWindow::invalidValueSettedByUserToTreeViewModel);

        QAbstractItemModel *model = ui->tagsTreeView->model();
        model->deleteLater();
        ui->tagsTreeView->setModel(newModel);
    }
}

void MainWindow::saveFile(const QString& filename)
{
    TagStoreHelperXML helper(filename);
    TagTreeModel *model = qobject_cast<TagTreeModel*>(ui->tagsTreeView->model());
    TagItem *root = model->getRoot();
    helper.store(root);
    if(helper.hasError()) {
        QMessageBox::warning(this, tr("Attention"), helper.errorString());
    }
}

void MainWindow::on_load_file_clicked(bool checked)
{
    Q_UNUSED(checked)
    const QString fileName = QFileDialog::getOpenFileName(this,
                                                          tr("Open File"),
                                                 lastUsedFileName,
                                                 "XML files (*.xml)");
    if(!fileName.isNull() && !fileName.isEmpty()) {
        lastUsedFileName = fileName;
        loadFile(fileName);
    }
}

void MainWindow::on_save_file_clicked(bool checked)
{
    Q_UNUSED(checked)
    const QString fileName = QFileDialog::getSaveFileName(this,
                                                          tr("Open File"),
                                                 lastUsedFileName,
                                                 "XML files (*.xml)");
    if(!fileName.isNull() && !fileName.isEmpty()) {
        lastUsedFileName = fileName;
        saveFile(fileName);
    }
}

void MainWindow::on_new_file_clicked(bool checked)
{
    Q_UNUSED(checked)
    TagTreeModel *newModel = new TagTreeModel(this);
    newModel->setColorHelper(&color_helper);
    newModel->setTagChecker(&tag_checker);
    TagItem *root = HELPER::createExampleTagTree();
    newModel->setRoot(root);
    connect(newModel, &TagTreeModel::invalidValueSetted, this, &MainWindow::invalidValueSettedByUserToTreeViewModel);
    QAbstractItemModel *model = ui->tagsTreeView->model();
    model->deleteLater();
    ui->tagsTreeView->setModel(newModel);
}

void MainWindow::storeSettings()
{
    _settingsHelper.saveLastFileName(lastUsedFileName);
    _settingsHelper.saveMainWindowPosition(size(), pos());
}

void MainWindow::loadSettings()
{
    QString _lastFname = _settingsHelper.loadLastFileName();
    if((!_lastFname.isNull()) && (!_lastFname.isEmpty())) {
        lastUsedFileName = _lastFname;
    }
    this->setStyleSheet(_settingsHelper.loadFontStyleSheet());
    // Load MainWindow position and size
    QSize size;
    QPoint pos;
    _settingsHelper.loadMainWindowPosition(size, pos);
    resize(size);
    move(pos);
}

void MainWindow::on_about_triggered(bool checked)
{
    Q_UNUSED(checked)
    QMessageBox::about(this,
                       tr("About %1").arg(qApp->applicationName()),
                       tr("<h2>%1</h2>"
                          "<p>Written by Yuriy (Yuri) Astrov<br/>"
                          "Based on Qt 5<br/>"
                          "Licensed by GPLv3<br/>"
                          "Version: %2<br/>"
                          "URL: <a href='%3'>%3</a></p>").arg(qApp->applicationName(), APP_VERSION, APP_URL));
}

void MainWindow::on_set_font_triggered(bool checked)
{
    Q_UNUSED(checked)
    bool ok;
    int fontSize;
    QString fontFamily;
    _settingsHelper.loadFont(fontSize, fontFamily);
    QFont font = QFontDialog::getFont(
                    &ok, QFont(fontFamily, fontSize), this);
    if (ok) {
        _settingsHelper.saveFont(font.pointSize(), font.family());
        // Try to move it to helper failed.
        this->setStyleSheet(QString("font-size:%1pt;font-family:%2;").arg(fontSize).arg(fontFamily));
    } else {
        ;
    }
}

void MainWindow::customContextMenuRequestedForTreeView(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    QAction *removeAction = new QAction(tr("Remove"), this);
    connect(removeAction, &QAction::triggered, this, &MainWindow::on_treeview_remove_triggered);
    QAction *newAction = new QAction(tr("Add new to current"), this);
    connect(newAction, &QAction::triggered, this, &MainWindow::on_treeview_new_triggered);
    QAction *newToRootAction = new QAction(tr("Add new to root"), this);
    connect(newToRootAction, &QAction::triggered, this, &MainWindow::on_treeview_new_to_root_triggered);
    menu->addAction(removeAction);
    menu->addAction(newAction);
    menu->popup(ui->tagsTreeView->viewport()->mapToGlobal(pos));
}

void MainWindow::on_treeview_remove_triggered(bool checked)
{
    Q_UNUSED(checked)
    const QTreeView *treeV = ui->tagsTreeView;
    QAbstractItemModel *model = treeV->model();
    if(model == nullptr) return;
    QModelIndex index = treeV->currentIndex();
    model->removeRow(index.row(), index.parent());
    /*const QModelIndexList indexList = treeV->selectionModel()->selectedIndexes();
    const QAbstractItemModel *model = treeV->model();
    for(const QModelIndex &index: indexList) {
        model->removeRow();
    }
    */
}

void MainWindow::on_treeview_new_to_root_triggered(bool checked)
{
    QAbstractItemModel *model = ui->tagsTreeView->model();
    model->insertRow(0, QModelIndex());
}

void MainWindow::on_treeview_new_triggered(bool checked)
{
    Q_UNUSED(checked)
    const QTreeView *treeV = ui->tagsTreeView;
    QAbstractItemModel *model = treeV->model();
    if(model == nullptr) return;
    QModelIndex index = treeV->currentIndex();
    model->insertRow(0, index);
}

void MainWindow::on_treeview_up_triggered(bool checked)
{
    Q_UNUSED(checked)
    const QTreeView *treeV = ui->tagsTreeView;
    QAbstractItemModel *model = treeV->model();
    if(model == nullptr) return;
    const QModelIndex index = treeV->currentIndex();
    if(!index.isValid()) return;
    const int index_row = index.row();
    const QModelIndex parent = index.parent();
    if(index_row < 1) return;
    model->moveRow(parent, index_row, parent, index_row-1);
}

void MainWindow::on_treeview_down_triggered(bool checked)
{
    Q_UNUSED(checked)
    const QTreeView *treeV = ui->tagsTreeView;
    QAbstractItemModel *model = treeV->model();
    if(model == nullptr) return;
    const QModelIndex index = treeV->currentIndex();
    if(!index.isValid()) return;
    const int index_row = index.row();
    const QModelIndex parent = index.parent();
    model->moveRow(parent, index_row, parent, index_row+1);
}


void MainWindow::invalidValueSettedByUserToTreeViewModel(QString message)
{
    // User set incorrect value
    QMessageBox::warning(this, tr("Error!"), message, QMessageBox::Ok);
}
