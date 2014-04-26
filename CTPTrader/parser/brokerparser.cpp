#include "brokerparser.h"

BrokerParser::BrokerParser()
{
}

bool BrokerParser::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts)
{
    return false;
}

bool BrokerParser::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    return false;
}

bool BrokerParser::endDocument()
{
    return false;
}

bool BrokerParser::startDocument()
{
    return false;
}

bool BrokerParser::characters(const QString &ch)
{
    return false;
}
