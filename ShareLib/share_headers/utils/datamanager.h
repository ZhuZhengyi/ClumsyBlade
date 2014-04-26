#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QtCore>

#include "../../../DataAccessLayer/dataaccesslayer.h"
#include "addressinfo.h"

class DataManager : public QObject
{
    Q_OBJECT

protected:

    static DataManager *m_pInstance;
    DataManager(QObject *parent = 0);

public:

    bool initLog(QString fileName);
    bool writeLog(QString logMsg);
    void loadMatchList(QString);
    void loadFrontAddresses(QString);
    void loadDAL(QString);
    void unloadDAL();
    void loadScriptEngine(QString);

    static DataManager * GetInstance(QObject *parent = 0)
    {
        if(NULL == m_pInstance)
            m_pInstance = new DataManager(parent);
        return m_pInstance;
    }

    static void Release()   //release when close
    {
        if(NULL != m_pInstance)
        {
            delete m_pInstance;
            m_pInstance = NULL;
        }
    }

    QStringList GetKeyword()
    {
        return list;
    }

    QList<QRegExp> GetMatchList()
    {
        return matchList;
    }

    DataAccessLayer * getDAL()
    {
        return dal;
    }

    QList<AddressInfo> getMarketDataAddrList()
    {
        return marketDataAddrList;
    }

    QList<AddressInfo> getTradingAddrList()
    {
        return tradingAddrList;
    }

private:

    QList<AddressInfo> marketDataAddrList;
    QList<AddressInfo> tradingAddrList;
    DataAccessLayer * dal;
    QList<QRegExp> matchList;
    QStringList list;
    QString BrokerID;
    QString BrokerName;
    QString logFile;
	QMutex lock;
    
signals:
    
public slots:
    
};

#endif // DATAMANAGER_H
