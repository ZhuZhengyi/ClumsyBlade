#include "datamanager.h"
#include <QtXml>
#include "PVSConstant.h"

//#define LOG_FILE_PATH "../bin"
//#define BROKERS_PATH  "brokers.xml"
//#define APP_SETTING "pvsniper.ini"

DataManager * DataManager::m_pInstance = NULL;

DataManager::DataManager(QObject *parent) :
    QObject(parent)
{
    logFile = "";
    //QDir::setCurrent(LOG_FILE_PATH);
}

void DataManager::loadFrontAddresses(QString addrFile)
{
    AddressInfo info;

#ifdef TEST
    info.setName(tr("simMd"));
	info.setAddrType(MarketData);
    info.setIpAddr("tcp://asp-sim2-md1.financial-trading-platform.com:26213");
    marketDataAddrList += info;

    info.setName(tr("simTrading"));
    info.setAddrType(Trading);
    info.setIpAddr("tcp://asp-sim2-front1.financial-trading-platform.com:26205");
    tradingAddrList += info;
#endif
    QDomDocument doc("brokers");
    //qDebug() << QDir::currentPath();
    QFile file(addrFile);

    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();
    QDomNodeList brokers = docElem.elementsByTagName("broker");

    if(!brokers.isEmpty())
    {
        QDomElement broker = brokers.at(0).toElement();
        BrokerID = broker.attribute("BrokerID");
        BrokerName = broker.attribute("BrokerName");
    }

    QDomNodeList servers = docElem.elementsByTagName("Server");

    if(!servers.isEmpty())
    {
        for(int i = 0; i < 3; i++)
        {
            QDomElement node = servers.at(i).toElement();
            QDomNodeList n2 = node.elementsByTagName("Name");
            if(!n2.isEmpty())
            {
                QDomElement n3 = n2.at(0).toElement();
                QString nodeTxt = n3.text();
                //QString nodeTxt = QTextCodec::codecForName("UTF-8")->fromUnicode(
                //            QTextCodec::codecForName("GB2312")->toUnicode(n3.text().toAscii()));
                //qDebug() << "out->" << nodeTxt; // the node really is an element.
                QDomNodeList n4 = node.elementsByTagName("Trading");
                if(!n4.isEmpty())
                {
                    QDomElement tradingList = n4.at(0).toElement();
                    QDomNodeList tradingListItems = tradingList.elementsByTagName("item");
                    for(int j = 0; j < tradingListItems.count(); j++)
                    {
                        info.setName(QString(nodeTxt + QString::number(j + 1)));
                        info.setAddrType(Trading);
                        info.setIpAddr(QString("tcp://" + tradingListItems.at(j).toElement().text()));
                        tradingAddrList += info;
                    }
                }

                n4 = node.elementsByTagName("MarketData");
                if(!n4.isEmpty())
                {
                    QDomElement tradingList = n4.at(0).toElement();
                    QDomNodeList tradingListItems = tradingList.elementsByTagName("item");
                    for(int j = 0; j < tradingListItems.count(); j++)
                    {
                        info.setName(QString(nodeTxt + QString::number(j + 1)));
                        info.setAddrType(Trading);
                        info.setIpAddr(QString("tcp://" + tradingListItems.at(j).toElement().text()));
                        marketDataAddrList += info;
                    }
                }
            }
        }
    }
}

bool DataManager::initLog(QString fileName)
{
	lock.lock();
    this->logFile = fileName;
    QFile f(logFile);
    if (!f.open(QIODevice::WriteOnly))
        return false;
    f.close();
	lock.unlock();
    return true;
}

bool DataManager::writeLog(QString logMsg)
{
	lock.lock();
    QFile f(logFile);
    if (!f.open(QIODevice::Append))
        return false;
    QTextStream ts(&f);
    ts << QDateTime::currentDateTime().toString("yyyy/MM/dd HH:mm:ss zzz") << "\t" << logMsg << "\r\n";
    //ts << "\t" << logMsg << "\r\n";
    f.close();
	lock.unlock();
    return true;
}

void DataManager::loadMatchList(QString matchFile)
{
    QFile matches(matchFile);

    if (!matches.open(QIODevice::ReadOnly))
    {
        qCritical() << "can't open match list file!";
        return;
    }

    QTextStream stream(&matches);
    while (!stream.atEnd())
    {
        matchList.push_back(QRegExp(stream.readLine()));
    }

    matches.close();
}

void DataManager::loadDAL(QString conf)
{
#ifdef unix
    QPluginLoader pluginLoader("libDataAccessLayer.so");
#else
    QPluginLoader pluginLoader("DataAccessLayer.dll");
#endif
    QObject * plugin = pluginLoader.instance();
    dal = qobject_cast<DataAccessLayer *>(plugin);

	if(!dal){
		qDebug() << "initDB failed!";
		return;
	}

    QSettings settings(conf, QSettings::IniFormat);

    QString ipAddr = settings.value("db_server_ip", "127.0.0.1").toString();

    //qDebug() << "ipAddr = " << ipAddr;

    dal->initDB("mydb.dat");
}

void DataManager::unloadDAL()
{
    if (dal)
    {
        dal->closeDB();
    }
}

void DataManager::loadScriptEngine(QString scriptMod)
{
   // QString filename = (QDir::currentPath() + QDir::separator() + "GmodLua.xml");

    QFile *data = new QFile(scriptMod);

    QDomDocument domDocument;
    QString errorStr;
    int errorLine;
    int errorColumn;

    //qDebug()<<"before Parse xml" << data->fileName();

    if (!domDocument.setContent(data, true, &errorStr, &errorLine,
                                &errorColumn))
    {
        qDebug()<<"Parse error at line " << errorLine
               << ", column " << errorColumn << ":\n" << errorStr;
        return;
    }

    //qDebug()<<"Parse xml";

    QDomElement root = domDocument.documentElement();

    //qDebug()<<"Parse xml rootcount=" << root.childNodes().count();

    QDomElement lan = root.firstChildElement("Languages").firstChildElement("Language");
    QDomElement keyword = lan.firstChildElement("Keywords");

    QString tempRes;

    while(!keyword.isNull())
    {
        tempRes += keyword.text() + " ";
        keyword = keyword.nextSiblingElement("Keywords");
    }

    list = tempRes.trimmed().split(" ");

    //qDebug() << list.count();

    data->close();

    delete data;
}


