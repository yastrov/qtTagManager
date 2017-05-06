#include "tagcolorhelperdefined.h"

TagColorHelperDefinedColors::TagColorHelperDefinedColors():
    _white(QColor(Qt::white)), _i_count(0), max_color(0)
{
    _colors << QColor(Qt::red) << QColor(Qt::green)<< QColor(Qt::blue)<<
                        QColor(Qt::cyan)<< QColor(Qt::magenta)<< QColor(Qt::yellow)<<
                        QColor(Qt::lightGray)<< QColor(Qt::gray);
    max_color = _colors.count();
}

void TagColorHelperDefinedColors::init(TagItem *root)
{
    _walkOverNode(root);
}

void TagColorHelperDefinedColors::addNew(const QString &value)
{
    const int count = _counter.value(value);
    if(count == 1) {
        _colors_hash[value] = _getNewUniq();
    }
    _counter[value] = count + 1;
}

void TagColorHelperDefinedColors::rename(const QString &oldValue, const QString &newValue)
{
    const int count = _counter.value(oldValue);
    if(count > 0) {
        _counter[oldValue] = count - 1;
        if(count == 1) {
            _colors_hash.remove(oldValue);
        }
    }
    addNew(newValue);
}

QColor TagColorHelperDefinedColors::fetchColor(const QString &value) const
{
    const int count = _counter.value(value);
    if(count > 1) {
        const int color = _colors_hash.value(value);
        return _colors.at(color);
    }
    return _white;
}

void TagColorHelperDefinedColors::_walkOverNode(TagItem *item)
{
    const QString tag_name = item->name();
    addNew(tag_name);
    for (TagItem *child: item->children())
        _walkOverNode(child);
}

int TagColorHelperDefinedColors::_getNewUniq()
{
    const int c = _i_count;
    _i_count = (_i_count + 1) % max_color;
    return c;
}
