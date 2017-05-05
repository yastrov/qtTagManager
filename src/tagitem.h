#ifndef TAGITEM_H
#define TAGITEM_H
#include <QString>
#include <QObject>

class TagItem
{
public:
    explicit TagItem(const QString &name=QString(),
                          TagItem *parent=nullptr);
    virtual ~TagItem() { qDeleteAll(m_children); }
    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }
    TagItem *parent() const { return m_parent; }
    TagItem *childAt(int row) const { return m_children.value(row); }
    int rowOfChild(TagItem *child) const
                    { return m_children.indexOf(child); }
    int row() const;
    int childCount() const { return m_children.count(); }
    bool hasChildren() const { return !m_children.isEmpty(); }
    QList<TagItem*> children() const { return m_children; }

    void insertChild(int row, TagItem *item) { item->m_parent = this;
                                            m_children.insert(row, item); }
    void addChild(TagItem *item) { item->m_parent = this;
                                    m_children << item; }
    void swapChildren(int oldRow, int newRow)
                    { m_children.swap(oldRow, newRow); }
    TagItem* takeChild(int row);
private:
    QString m_name;
    TagItem *m_parent;
    QList<TagItem*> m_children;
};

#endif // TAGITEM_H
