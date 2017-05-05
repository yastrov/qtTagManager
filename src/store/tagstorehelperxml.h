#ifndef TAGSTOREHELPERXML_H
#define TAGSTOREHELPERXML_H
#include <QString>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include "abstracttagstorehelper.h"
#include "../tagitem.h"

class TagStoreHelperXML : public AbstractTagStoreHelper
{
public:
    explicit TagStoreHelperXML():AbstractTagStoreHelper() {}
    explicit TagStoreHelperXML(const QString &fname): AbstractTagStoreHelper(fname) {}
    virtual ~TagStoreHelperXML() {}

    TagItem * _loadNodesViaStream(QXmlStreamReader *reader);
    void _storeNodesViaStream(QXmlStreamWriter *writer, TagItem *item);
private:
    TagItem *loadF() override;
    void storeF(TagItem *root) override;
};

#endif // TAGSTOREHELPERXML_H
