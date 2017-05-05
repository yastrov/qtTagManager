#ifndef TAGTREEMODEL_H
#define TAGTREEMODEL_H

#include <QAbstractItemModel>
#include <QFontMetrics>
#include <QStyleOptionComboBox>
#include <QApplication>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QXmlStreamWriter>
#include <QMessageBox>
#include <QMimeData>
#include "tagitem.h"
#include "abstracttagcolorhelper.h"
#include "validator/abstracttagvalidcheckhelper.h"
#include <QDebug>

class TagTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TagTreeModel(QObject *parent = nullptr);
    virtual ~TagTreeModel() {delete rootItem;}

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    //---
    bool insertRow(int row, const QModelIndex &parent);
    bool insertRows(int row, int count, const QModelIndex &parent) override;
    bool removeRow(int row, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent) override;
    bool moveRow(const QModelIndex &sourceParent, int sourceRow, const QModelIndex &destinationParent, int destinationChild);
    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;
    //--- Drag'n'Drop
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *mimeData,
                        Qt::DropAction action, int row, int column,
                        const QModelIndex &parent);
    static void readTags(QXmlStreamReader *reader, TagItem *item);
    static void writeTagAndChildren(QXmlStreamWriter *writer,
                                  TagItem *item);
    Qt::DropActions supportedDragActions() const override
            { return Qt::MoveAction; }
    Qt::DropActions supportedDropActions() const override
            { return Qt::MoveAction; }
    //----- Support
    TagItem *getRoot() const {return rootItem;}
    void setRoot(TagItem *root) {rootItem = root; if(color_helper) color_helper->init(root);}
    //----- Support
    QStringList pathForIndex(const QModelIndex &index) const;
    QModelIndex indexForPath(const QStringList &path) const;
    //-----
    void setColorHelper(AbstractTagColorHelper *helper) {color_helper = helper;}
    AbstractTagColorHelper *getColorHelper() {return color_helper;}
    void setTagChecker(AbstractTagValidCheckHelper *checker) {tag_checker = checker;}
    AbstractTagValidCheckHelper *getTagChecker() {return tag_checker;}

private:
    TagItem *itemForIndex(const QModelIndex &index) const;
    QModelIndex indexForPath(const QModelIndex &parent,
                             const QStringList &path) const;

    TagItem *rootItem;
    AbstractTagColorHelper *color_helper;
    AbstractTagValidCheckHelper *tag_checker;
#ifdef QML_MODEL
    QHash<int, QByteArray> m_roleNameMapping;

    QHash<int, QByteArray> roleNames() const override;
    enum TreeModel_Roles
    {
        Role_NAME  = Qt::UserRole,
        Role_COLOR = Qt::UserRole+1,
        Role_TIP = Qt::UserRole+2
    };
#endif
};

#endif // TAGTREEMODEL_H
