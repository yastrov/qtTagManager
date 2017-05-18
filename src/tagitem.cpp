#include "tagitem.h"

TagItem::TagItem(const QString &name,
                 TagItem *parent): m_parent(parent)
{
    if (m_parent)
       m_parent->addChild(this);
    m_name = name;
}

TagItem* TagItem::takeChild(int row)
{
    TagItem *item = m_children.takeAt(row);
    Q_ASSERT(item);
    item->m_parent = nullptr;
    return item;
}

bool TagItem::moveChild(int from, int to)
{
    const int children_len = m_children.length();
    if(m_children.empty() || from < 0 || to < 0 || to == from|| from > children_len)
        return false;
    if(to <= children_len && to < from) {
        m_children.move(from, to);
    } else {
        if(to > from) {
            TagItem *item = m_children.takeAt(from);
            if(to >= children_len) {
                m_children.append(item);
            }
            else {
                m_children.insert(to, item);
            }
        }
    }
    return true;
}

int TagItem::row() const
{
    if (m_parent)
        return m_parent->m_children.indexOf(const_cast<TagItem*>(this));
    return 0;
}

#ifdef MYPREFIX_DEBUG
QDebug operator <<(QDebug debug, const TagItem& object)
{
    debug << "< name: "<< object.name() << "\n"
          << " childs: " << object.childCount();
    for (TagItem* item: object.children()) {
        debug << item->name();
    }
    debug << ">";
    return debug;
}

QDebug operator <<(QDebug debug, TagItem* object)
{
    debug << "< name: "<< object->name() << "\n"
          << " childs: " << object->childCount();
    for (TagItem* item: object->children()) {
        debug << item->name();
    }
    debug << ">";
    return debug;
}
#endif
