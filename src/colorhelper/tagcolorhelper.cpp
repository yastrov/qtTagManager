#include "tagcolorhelper.h"

TagColorHelper::TagColorHelper():
    _white(QColor(Qt::white)), _i_count(0), max_color(360)
{

}

void TagColorHelper::init(TagItem *root)
{
    _walkOverNode(root);
}

void TagColorHelper::addNew(const QString &value)
{
    const int count = _counter.value(value);
    if(count == 1) {
        _colors_hash[value] = _getNewUniq();
    }
    _counter[value] = count + 1;
}

void TagColorHelper::rename(const QString &oldValue, const QString &newValue)
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

QColor TagColorHelper::fetchColor(const QString &value) const
{
    const int count = _counter.value(value);
    if(count > 1) {
        const int color = _colors_hash.value(value);
        return QColor::fromHsv(color, 70, 70);
    }
    return _white;
}

void TagColorHelper::_walkOverNode(TagItem *item)
{
    const QString tag_name = item->name();
    addNew(tag_name);
    for (TagItem *child: item->children())
        _walkOverNode(child);
}

int TagColorHelper::_getNewUniq()
{
    const int c = _i_count;
    _i_count = (_i_count + 10) % max_color;
    return c;
}
