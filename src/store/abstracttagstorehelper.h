#ifndef ABSTRACTTAGSTOREHELPER_H
#define ABSTRACTTAGSTOREHELPER_H
#include <QString>
#include <QFile>
#include <QDir>
#include "../tagitem.h"

class AbstractTagStoreHelper
{
public:
    explicit AbstractTagStoreHelper();
    explicit AbstractTagStoreHelper(const QString &fname);
    virtual ~AbstractTagStoreHelper() {}

    virtual TagItem *load();
    virtual void store(TagItem *root);

    void setFileName(const QString &fname) {_fname = fname;}
    QString getFileName() const {return _fname;}
    bool hasError() const {return _hasError;}
    QString errorString() const {return _errorMsg;}
    void resetError() {_hasError=false; _errorMsg="";}
protected:
    virtual TagItem *loadF() {return nullptr;}
    virtual void storeF(TagItem *root) {
        Q_UNUSED(root)
    }

    bool _hasError;
    QString _errorMsg;
    QString _fname;
};

#endif // ABSTRACTTAGSTOREHELPER_H
