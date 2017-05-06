#ifndef ABSTRACTTAGCOLORHELPER_H
#define ABSTRACTTAGCOLORHELPER_H
#include <QColor>
#include <QString>
#include "../tagitem.h"

class AbstractTagColorHelper
{
public:
    explicit AbstractTagColorHelper() {}
    virtual ~AbstractTagColorHelper() {}
    virtual void init(TagItem *root) = 0;
    virtual void addNew(const QString &value) = 0;
    virtual void rename(const QString &oldValue, const QString &newValue) = 0;
    virtual QColor fetchColor(const QString &value) const = 0;
};

#endif // ABSTRACTTAGCOLORHELPER_H
