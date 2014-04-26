#ifndef BROKERPARSER_H
#define BROKERPARSER_H

#include <QtXml>

class BrokerParser : public QXmlDefaultHandler
{
public:
    BrokerParser();

    bool startDocument();

    bool startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);

    bool characters(const QString &ch);

    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);

    bool endDocument();
};

#endif // BROKERPARSER_H
