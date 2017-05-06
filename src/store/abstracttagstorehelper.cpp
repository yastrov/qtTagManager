#include "abstracttagstorehelper.h"

AbstractTagStoreHelper::AbstractTagStoreHelper()
    :_hasError(false), _errorMsg("")
{

}

AbstractTagStoreHelper::AbstractTagStoreHelper(const QString &fname)
    :AbstractTagStoreHelper()
{
    _fname = fname;
}

TagItem *AbstractTagStoreHelper::load()
{
    if(_fname.isNull() || _fname.isEmpty()) {
        _errorMsg = QObject::tr("Invalid filename");
        _hasError = true;
        return nullptr;
    }
    QFile file(_fname);
    QFileInfo finfo(file);
    if(!file.exists() && !finfo.isFile()) {
        _errorMsg = QObject::tr("File not exists!");
        _hasError = true;
        return nullptr;
    }
    return loadF();
}

void AbstractTagStoreHelper::store(TagItem *root)
{
    if(_fname.isNull() || _fname.isEmpty()) {
        _errorMsg = QObject::tr("Invalid filename");
        _hasError = true;
        return;
    }
    if(!root) {
        _errorMsg = QObject::tr("Invalid Tag data!");
        _hasError = true;
        return;
    }
    storeF(root);
}
