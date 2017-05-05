#include "tagstorehelperxml.h"

namespace {
const QString TagTag = "item";
const QString TagNameAttr = "tag_name";
}

TagItem *TagStoreHelperXML::loadF()
{
    TagItem *root;
    QFile file(_fname);
    if(file.open(QIODevice::ReadOnly)) {
        QXmlStreamReader reader(&file);
        root = _loadNodesViaStream(&reader);
        if(reader.hasError()) {
            _errorMsg = reader.errorString();
            _hasError = true;
            file.close();
            return nullptr;
        }
    } else {
        _errorMsg = QObject::tr("Can't open file! %1").arg(file.errorString());
        _hasError = true;
        return nullptr;
    }
    file.close();
    return root;
}

void TagStoreHelperXML::storeF(TagItem *root)
{
    QFile file(_fname);
    if(file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QXmlStreamWriter writer(&file);
        writer.setAutoFormatting(true);
        writer.writeStartDocument();
        writer.writeStartElement("items");
        _storeNodesViaStream(&writer, root);
        writer.writeEndElement();
        writer.writeEndDocument();
        if(writer.hasError()) {
            _errorMsg = QObject::tr("Can't open file! %1").arg(file.errorString());
            _hasError = true;
            file.close();
            return;
        }
    } else {
        _errorMsg = QObject::tr("Can't open file! ") + file.errorString();
        _hasError = true;
    }
    file.close();
}

TagItem * TagStoreHelperXML::_loadNodesViaStream(QXmlStreamReader *reader)
{
    TagItem *item = nullptr;
    while (!reader->atEnd()) {
            reader->readNext();
            if (reader->isStartElement()) {
                if (reader->name() == TagTag) {
                    const QString tag_name = reader->attributes()
                            .value(TagNameAttr).toString();
                    item = new TagItem(tag_name, item);
                }
            } else {
                if(reader->isEndElement())
                {
                    if (reader->name() == TagTag) {
                        TagItem *parent = item->parent();
                        if(parent)
                            item = parent;
                    }
                }
            }
        }
    return item;
}

void TagStoreHelperXML::_storeNodesViaStream(QXmlStreamWriter *writer, TagItem *item)
{
    writer->writeStartElement(TagTag);
    writer->writeAttribute(TagNameAttr, item->name());
    for (TagItem *child: item->children())
        _storeNodesViaStream(writer, child);
    //if (item != rootItem)
    writer->writeEndElement();
}
