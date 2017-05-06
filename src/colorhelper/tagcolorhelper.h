#ifndef TAGCOLORHELPER_H
#define TAGCOLORHELPER_H
#include <QString>
#include <QHash>
#include <QList>
#include <QColor>
#include "../tagitem.h"
#include "abstracttagcolorhelper.h"

class TagColorHelper: public AbstractTagColorHelper
{
public:
    explicit TagColorHelper();
    virtual ~TagColorHelper() {}

    void init(TagItem *root) override;
    void addNew(const QString &value) override;
    void rename(const QString &oldValue, const QString &newValue) override;
    QColor fetchColor(const QString &value) const override;
private:
    void _walkOverNode(TagItem *root);
    int _getNewUniq();

    QHash<QString, int> _counter;
    QHash<QString, int> _colors_hash;
//QList<QString> _tags;
    QColor _white;
    int _i_count;
    int max_color;
};

#endif // TAGCOLORHELPER_H
