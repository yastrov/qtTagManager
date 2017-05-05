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

int TagItem::row() const
{
    if (m_parent)
        return m_parent->m_children.indexOf(const_cast<TagItem*>(this));
    return 0;
}
