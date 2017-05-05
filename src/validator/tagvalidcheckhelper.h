#ifndef TAGVALIDCHECKHELPER_H
#define TAGVALIDCHECKHELPER_H
#include "abstracttagvalidcheckhelper.h"
#include <QString>
#include <QStringList>

class TagValidCheckHelper : public AbstractTagValidCheckHelper
{
public:
    TagValidCheckHelper();
    virtual ~TagValidCheckHelper() {}
    bool check(const QString &value) const override;
    QString errorMsg() const override;
};

#endif // TAGVALIDCHECKHELPER_H
