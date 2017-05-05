#ifndef ABSTRACTTAGVALIDCHECKHELPER_H
#define ABSTRACTTAGVALIDCHECKHELPER_H
#include <QString>

class AbstractTagValidCheckHelper
{
public:
    AbstractTagValidCheckHelper() {}
    virtual ~AbstractTagValidCheckHelper() {}
    virtual bool check(const QString &value) const {
        Q_UNUSED(value)
        return true;
    }
    virtual QString errorMsg() const {return "";}
};

#endif // ABSTRACTTAGVALIDCHECKHELPER_H
