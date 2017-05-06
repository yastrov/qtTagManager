#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QFontDialog>
#include <QFont>
#include <QAbstractItemModel>
#include "tagtreemodel.h"
#include "tagitem.h"
#include "store/tagstorehelperxml.h"
#include "store/abstracttagstorehelper.h"
#include "tagcolorhelper.h"
#include "validator/abstracttagvalidcheckhelper.h"
#include "validator/tagvalidcheckhelper.h"
#include "settingshelper.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

    void loadFile(const QString& filename);
    void saveFile(const QString& filename);

    void storeSettings();
    void loadSettings();

private slots:
    void on_about_qt_triggered(bool checked) {
        Q_UNUSED(checked)
        QMessageBox::aboutQt(this);}
    void invalidValueSettedByUserToTreeViewModel(QString message);
    void on_about_triggered(bool checked);
    void on_load_file_clicked(bool checked);
    void on_save_file_clicked(bool checked);
    void on_new_file_clicked(bool checked);
    void on_set_font_triggered(bool checked);
    void customContextMenuRequestedForTreeView(const QPoint &pos);
    void on_treeview_remove_triggered(bool checked);
    void on_treeview_new_triggered(bool checked);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    TagColorHelper color_helper;
    TagValidCheckHelper tag_checker;
    QString lastUsedFileName;
    SettingsHelper _settingsHelper;
};

#endif // MAINWINDOW_H
