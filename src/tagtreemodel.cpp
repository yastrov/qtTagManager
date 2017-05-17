#include "tagtreemodel.h"

/*See example from:
From http://doc.qt.io/qt-5/qtwidgets-itemviews-simpletreemodel-example.html
or in your Qt Creator examples.
Use macro QML_MODEL for use QML.
*/

namespace {
const int ColumnCount = 1;
const int MaxCompression = 9;
enum Column {Name, };
const QString MimeType = "application/vnd.qtrac.xml.tagitem";
const QString TagTag = "item";
const QString TagNameAttr = "tag_name";
}

TagTreeModel::TagTreeModel(QObject *parent)
    : QAbstractItemModel(parent), color_helper(nullptr), tag_checker(nullptr)
{
    #ifdef QML_MODEL
    m_roleNameMapping[TreeModel_Roles::Role_NAME] = "name";
    m_roleNameMapping[TreeModel_Roles::Role_COLOR] = "color";
    m_roleNameMapping[TreeModel_Roles::Role_TIP] = "tip";
    #endif
}

QVariant TagTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == Name)
            return tr("Tag");
    }
    return QVariant();
}

bool TagTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

QModelIndex TagTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
       return QModelIndex();
    TagItem *parentItem = itemForIndex(parent);
    if(!parentItem)
       return QModelIndex();
    TagItem *childItem = parentItem->childAt(row);
    //if(childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

TagItem *TagTreeModel::itemForIndex(const QModelIndex &index) const
{
    if (index.isValid()) {
        TagItem *item = static_cast<TagItem*>(index.internalPointer());
        if (item) {
            return item;
        }
    }
    return rootItem;
}

QModelIndex TagTreeModel::parent(const QModelIndex &index) const
{
    /*From http://doc.qt.io/qt-5/qtwidgets-itemviews-simpletreemodel-example.html*/
    if (!index.isValid())
        return QModelIndex();
    TagItem *childItem = static_cast<TagItem*>(index.internalPointer());
    if(!childItem)
        return QModelIndex();
    TagItem *parentItem = childItem->parent();
    if (parentItem == rootItem)
        return QModelIndex();
    return createIndex(parentItem->row(), 0, parentItem);
}

int TagTreeModel::rowCount(const QModelIndex &parent) const
{
    /*From http://doc.qt.io/qt-5/qtwidgets-itemviews-simpletreemodel-example.html*/
    if (parent.column() > 0)
        return 0;
    TagItem *parentItem = itemForIndex(parent);
    if(parentItem)
        return parentItem->childCount();
    return 0;
}

int TagTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    if(rootItem)
        return ColumnCount;
    return 0;
}

QVariant TagTreeModel::data(const QModelIndex &index, int role) const
{
    qDebug()<<"TagTreeModel::data";
    if (!index.isValid())
        return QVariant();
    const int column = index.column();
    if(!rootItem || column < 0 || column >= ColumnCount)
        return QVariant();
    TagItem *item = itemForIndex(index);
    if(!item) return QVariant();
    #ifdef QML_MODEL
        switch (role) {
        case TreeModel_Roles::Role_NAME:
            return item->name();
            break;
        case TreeModel_Roles::Role_COLOR:
            if(color_helper)
                return color_helper->fetchColor(item->name());
            else return QColor(Qt::white);
            break;
        case TreeModel_Roles::Role_TIP:
            return item->name();
            break;
        default:
            return QVariant();
            break;
        }
    #else
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
    case Qt::ToolTipRole:
        switch (column) {
            case Name: return item->name();
            default: Q_ASSERT(false);
        }
        break;
    case Qt::TextAlignmentRole:
        if (column == Name) {
            return static_cast<int>(Qt::AlignVCenter|
                                    Qt::AlignLeft);
        } else {
            return static_cast<int>(Qt::AlignVCenter|Qt::AlignRight);
        }
        break;
    case Qt::BackgroundColorRole:
        if(color_helper)
            return color_helper->fetchColor(item->name());
        else return QColor(Qt::white);
        break;
    default:
        return QVariant();
        break;
    }
    #endif
    return QVariant();
}

bool TagTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug()<<"TagTreeModel::setData";
    if (data(index, role) != value) {
        // FIXME: Implement me!
        if (!rootItem || !index.isValid())
            return false;
        TagItem *item = itemForIndex(index);
        if (!item) return false;
        #ifdef QML_MODEL
        switch (role) {
        case TreeModel_Roles::Role_NAME:
            const QString name = item->name();
            const QString new_name = value.toString();
            if(tag_checker) {
                if(tag_checker->check(new_name) == false) {
                    emit invalidValueSetted(tag_checker->errorMsg());
                    return false;
                }
            }
            if(color_helper) color_helper->rename(name, new_name);
            item->setName(new_name);
            break;
        default:
            return false;
            break;
        }
        #else
        const int column = index.column();
        switch (role) {
        case Qt::EditRole:
            switch (column) {
                case Name:{
                const QString name = item->name();
                const QString new_name = value.toString();
                if(tag_checker) {
                    if(tag_checker->check(new_name) == false) {
                        emit invalidValueSetted(tag_checker->errorMsg());
                        return false;
                    }
                }
                if(color_helper) color_helper->rename(name, new_name);
                item->setName(new_name);
                }
                break;
                default: Q_ASSERT(false);
            }
            break;
        default:
            break;
        }
        #endif
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags TagTreeModel::flags(const QModelIndex &index) const
{
    qDebug()<<"TagTreeModel::flags";
    Qt::ItemFlags theFlags = QAbstractItemModel::flags(index);
    if (index.isValid())
    theFlags |= Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsEditable|
                Qt::ItemIsDragEnabled|Qt::ItemIsDropEnabled;
    return theFlags;
}

//-------------------------------
bool TagTreeModel::insertRow(int row, const QModelIndex &parent)
{
    qDebug()<<"TagTreeModel::insertRow";
    return insertRows(row, 1, parent);
}

bool TagTreeModel::insertRows(int row, int count,
                           const QModelIndex &parent)
{
    qDebug()<<"TagTreeModel::insertRows";
    if (!rootItem)
        rootItem = new TagItem;
    TagItem *parentItem = itemForIndex(parent);
    TagItem *item;
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i) {
        if(color_helper) color_helper->addNew(tr("New Tag"));
        item = new TagItem(tr("New Tag"));
        parentItem->insertChild(row, item);
    }
    endInsertRows();
    return true;
}

bool TagTreeModel::removeRow(int row, const QModelIndex &parent)
{
    qDebug()<<"TagTreeModel::removeRow";
    return removeRows(row, 1, parent);
}

bool TagTreeModel::removeRows(int row, int count,
                           const QModelIndex &parent)
{
    if (!rootItem)
        return false;
    TagItem *item = itemForIndex(parent);
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
        delete item->takeChild(row);
    endRemoveRows();
    return true;
}

bool TagTreeModel::moveRow(const QModelIndex &sourceParent, int sourceRow, const QModelIndex &destinationParent, int destinationChild)
{
    qDebug()<<"TagTreeModel::moveRow";
    return moveRows(sourceParent, sourceRow, 1, destinationParent, destinationChild);
}

bool TagTreeModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    qDebug()<<"TagTreeModel::moveRows";
    if(!sourceParent.isValid() || !destinationParent.isValid())
        return false;
    TagItem *s_p_item = static_cast<TagItem*>(sourceParent.internalPointer());
    TagItem *d_p_item = static_cast<TagItem*>(destinationParent.internalPointer());
    TagItem *item;
    beginMoveRows(sourceParent, sourceRow, sourceRow+count-1, destinationParent, destinationChild);
    for(int i=0; i<count; ++i) {
        item = s_p_item->childAt(sourceRow);
        d_p_item->insertChild(destinationChild+i, item);
    }
    endMoveRows();
    return true;
}

//---Drag'n'Drop

QStringList TagTreeModel::mimeTypes() const
{
    return QStringList() << MimeType;
}

QMimeData *TagTreeModel::mimeData(const QModelIndexList &indexes) const
{
    qDebug()<<"TagTreeModel::mimeData";
    Q_ASSERT(indexes.count());
    if (indexes.count() != 1)
        return nullptr;
    if (TagItem *item = itemForIndex(indexes.at(0))) {
        QMimeData *mimeData = new QMimeData;
        QByteArray xmlData;
        QXmlStreamWriter writer(&xmlData);
        writeTagAndChildren(&writer, item);
        mimeData->setData(MimeType, qCompress(xmlData,
                                              MaxCompression));
        return mimeData;
    }
    return nullptr;
}

bool TagTreeModel::dropMimeData(const QMimeData *mimeData,
        Qt::DropAction action, int row, int column,
        const QModelIndex &parent)
{
    qDebug()<<"TagTreeModel::dropMimeData";
    if (action == Qt::IgnoreAction)
        return true;
    if (action != Qt::MoveAction || column > 0 ||
        !mimeData || !mimeData->hasFormat(MimeType))
        return false;
    if (TagItem *item = itemForIndex(parent)) {
        QByteArray xmlData = qUncompress(mimeData->data(MimeType));
        QXmlStreamReader reader(xmlData);
        if (row == -1)
            row = parent.isValid() ? parent.row()
                                   : rootItem->childCount();
        beginInsertRows(parent, row, row);
        readTags(&reader, item);
        endInsertRows();
        return true;
    }
    return false;
}

void TagTreeModel::readTags(QXmlStreamReader *reader, TagItem *item)
{
    qDebug()<<"TagTreeModel::readTags";
    while (!reader->atEnd()) {
        reader->readNext();
        if (reader->isStartElement()) {
            if (reader->name() == TagTag) {
                const QString tag_name = reader->attributes()
                        .value(TagNameAttr).toString();
                item = new TagItem(tag_name, item);
            }
        } else {
            if(reader->isEndElement())
            {
                if (reader->name() == TagTag) {
                    TagItem *parent = item->parent();
                    if(parent)
                        item = parent;
                }
            }
        }
    }
}

void TagTreeModel::writeTagAndChildren(QXmlStreamWriter *writer,
                                  TagItem *item)
{
    qDebug()<<"TagTreeModel::writeTagAndChildren";
    writer->writeStartElement(TagTag);
    writer->writeAttribute(TagNameAttr, item->name());
    for (TagItem *child: item->children())
        writeTagAndChildren(writer, child);
    //if (item != rootItem)
    writer->writeEndElement();
}


//----------------------

#ifdef QML_MODEL
QHash<int, QByteArray> TagTreeModel::roleNames() const
{
    return m_roleNameMapping;
}
#endif

//---------------
// HELPERS

QStringList TagTreeModel::pathForIndex(const QModelIndex &index) const
{
    QStringList path;
    QModelIndex thisIndex = index;
    while (thisIndex.isValid()) {
        path.prepend(data(thisIndex).toString());
        thisIndex = thisIndex.parent();
    }
    return path;
}


QModelIndex TagTreeModel::indexForPath(const QStringList &path) const
{
    return indexForPath(QModelIndex(), path);
}

QModelIndex TagTreeModel::indexForPath(const QModelIndex &parent,
                                    const QStringList &path) const
{
    if (path.isEmpty())
        return QModelIndex();
    const int _row_count = rowCount(parent);
    const QString _path_0 = path.at(0);
    for (int row = 0; row < _row_count; ++row) {
        QModelIndex thisIndex = index(row, 0, parent);
        if (data(thisIndex).toString() == _path_0) {
            if (path.count() == 1)
                return thisIndex;
            thisIndex = indexForPath(thisIndex, path.mid(1));
            if (thisIndex.isValid())
                return thisIndex;
        }
    }
    return QModelIndex();
}
