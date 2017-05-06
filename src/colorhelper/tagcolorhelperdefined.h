#ifndef TAGCOLORHELPERDEFINEDCOLORS_H
#define TAGCOLORHELPERDEFINEDCOLORS_H
#include <QString>
#include <QHash>
#include <QList>
#include <QColor>
#include "../tagitem.h"
#include "abstracttagcolorhelper.h"

class TagColorHelperDefinedColors: public AbstractTagColorHelper
{
public:
    explicit TagColorHelperDefinedColors();
    virtual ~TagColorHelperDefinedColors() {}

    void init(TagItem *root) override;
    void addNew(const QString &value) override;
    void rename(const QString &oldValue, const QString &newValue) override;
    QColor fetchColor(const QString &value) const override;
private:
    void _walkOverNode(TagItem *root);
    int _getNewUniq();

    QHash<QString, int> _counter;
    QHash<QString, int> _colors_hash;
    QList<QColor> _colors;
    QColor _white;
    int _i_count;
    int max_color;
};

#endif // TAGCOLORHELPERDEFINEDCOLORS_H
