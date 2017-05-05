#include "tagvalidcheckhelper.h"
#include <QObject>

TagValidCheckHelper::TagValidCheckHelper():
    AbstractTagValidCheckHelper()
{

}

bool TagValidCheckHelper::check(const QString &value) const
{
    if(value.isNull()) return false;
    if(value.isEmpty()) return false;
    const int len = value.length();
    if(len > 10 || len < 2)
        return false;
    QStringList words = value.split(" ");
    const int count_of_words = words.count();
    if(count_of_words > 4)
        return false;
    return true;
}

QString TagValidCheckHelper::errorMsg() const
{
    return QObject::tr("Value must be between 2 and 10 symbols, and maximum 4 words.");
}
