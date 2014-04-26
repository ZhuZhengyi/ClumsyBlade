#include "ctpmdwrapper.h"
#include "ctptraderwrapper.h"

#include "../ShareLib/share_headers/data_header/CommonStrut.h"
#include "../ShareLib/share_headers/utils/datamanager.h"
#include "../ShareLib/share_headers/utils/StringUtil.h"
#include "../ShareLib/share_headers/utils/filesystemhelper.h"


char FRONT_ADDR_MD[101];
extern char FRONT_ADDR_TRADER[];

TThostFtdcBrokerIDType	BROKER_ID;
TThostFtdcInvestorIDType INVESTOR_ID;
TThostFtdcPasswordType  PASSWORD;
int iRequestID;

// UserApi
CThostFtdcMdApi* pMdApi;
extern CThostFtdcTraderApi* pTraderApi;


void MdThread::run()
{
    if(!pMdApi){
        qDebug() << __FUNCTION__ <<"pMdApi is null";
        return;
    }

    pMdApi->RegisterFront(FRONT_ADDR_MD);
    pMdApi->Init();
    pMdApi->Join();

}

CTPMdWrapper::CTPMdWrapper(QObject *parent)
    :CTPWrapper(parent)
{
    initRes();
}

CTPMdWrapper::~CTPMdWrapper()
{
    releaseRes();
}

void CTPMdWrapper::initRes()
{
    createDirInNotExist(PVSniper::MD_CON);

    pMdApi = CThostFtdcMdApi::CreateFtdcMdApi(PVSniper::MD_CON.toAscii().constData());
    pMdApi->RegisterSpi(this);
}

void CTPMdWrapper::releaseRes()
{
    if (pMdApi)
    {
        pMdApi->RegisterSpi(NULL);
        pMdApi->Release();
    }
}

void CTPMdWrapper::FrontConnect()
{
    MdThread *t = new MdThread();
    t->start();

    emit outputSignal(tr("connecting to md server"), false);
}

void CTPMdWrapper::OnFrontConnected()
{
    emit outputSignal(tr("connect to md server succeed!"), false);

    ReqUserLogin();
}

void CTPMdWrapper::ReqUserLogin()
{
    idList.clear();
    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));
    int iResult = pMdApi->ReqUserLogin(&req, ++iRequestID);

    emit outputSignal(tr("login to md server..."), false);
}

void CTPMdWrapper::OnRspError(CThostFtdcRspInfoField *pRspInfo,
                              int nRequestID, bool bIsLast)
{
    emit outputSignal("--->>> OnRspError \n");
    IsErrorRspInfo(pRspInfo);
}

void CTPMdWrapper::OnFrontDisconnected(int nReason)
{
    QString str;
    str.append(__FUNCTION__);
    str.append("\n nReason=" + nReason);
    str.append("\n");
    emit outputSignal(str);

}

void CTPMdWrapper::OnHeartBeatWarning(int nTimeLapse)
{
    QString str;
    str.append(__FUNCTION__);
    str.append("\n nTimeLapse=" + nTimeLapse);
    str.append("\n");
    emit outputSignal(str);
}

void CTPMdWrapper::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
                                  CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    if (!bIsLast || IsErrorRspInfo(pRspInfo))
    {
        qDebug() << __FUNCTION__ << pRspInfo->ErrorMsg;
        emit outputSignal(pRspInfo->ErrorMsg);
        emit onLoginSignal(false);
        return;
    }

    emit outputSignal(tr("login to md server succeed!", false), false);
    emit onLoginSignal(true);

    //pMdApi->

    SubscribeCurrentInstrumentInfo();
}

void CTPMdWrapper::SubscribeInstrumentInfo(QString InstrumentID, QString ExchangeID)
{
    bool findAMatch = false;

    Q_FOREACH(QRegExp re, DataManager::GetInstance()->GetMatchList())
    {
        findAMatch = re.exactMatch(InstrumentID);
        if(findAMatch)
            break;
    }

    if (!findAMatch)
    {
        return;
    }

    if(idList.indexOf(InstrumentID) == -1)
    {
        idList += InstrumentID;

        QByteArray ba = InstrumentID.toLatin1();
        const char *c_str2 = ba.data();
        char *ids[]  = {const_cast<char *>(c_str2)};
        pMdApi->SubscribeMarketData(ids, 1);
    }
}

// 查询自订列表信息
void CTPMdWrapper::SubscribeCurrentInstrumentInfo()
{
    QFile currentInstruments(PVSniper::CURRENT_INSTRUMENTS);
    if (!currentInstruments.open(QIODevice::ReadOnly))
    {
        qDebug() << "can't open file path=" << currentInstruments.fileName();
        return;
    }

    QTextStream ts(&currentInstruments);
    QMap<QString, QString> instruments_map;
    QString readLine = ts.readLine(30);

    while (readLine.trimmed() != "")
    {
        QStringList items = readLine.split(":");
        qDebug()<< __FUNCTION__ << "key=" << items.at(0) << " value=" << items.at(1);
        instruments_map.insert(items.at(0), items.at(1));
        readLine = ts.readLine(30);
    }

    currentInstruments.close();

    Q_FOREACH(QString item, instruments_map.keys())
    {
        SubscribeInstrumentInfo(item, instruments_map.value(item));
        emit addInstrumentSignal(item, instruments_map.value(item));
    }
}

void CTPMdWrapper::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    // qDebug() << Q_FUNC_INFO << pSpecificInstrument->InstrumentID << " error=" << pRspInfo->ErrorMsg;
    //qDebug() << __FUNCTION__ ;
    //pSpecificInstrument->InstrumentID
}

void CTPMdWrapper::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    qDebug() << __FUNCTION__;
}

void CTPMdWrapper::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
    //qDebug() << __FUNCTION__;

    TickDataStruct data;

    data.UpdateTime = QString(pDepthMarketData->UpdateTime);
    data.LastPrice = checkValue(pDepthMarketData->LastPrice).toDouble();
    data.Volume = checkValue(pDepthMarketData->Volume, "%d", 0).toInt();

    //qDebug() << "pDepthMarketData->Volume=" << pDepthMarketData->Volume;
    //qDebug() << "data.Volume=" << data.Volume;

    DataManager::GetInstance()->getDAL()->insertTickData(pDepthMarketData->InstrumentID, data);
    emit updateDataSignal(pDepthMarketData);
}

bool CTPMdWrapper::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{

    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult)
    {
        QString str;
        str.append(pRspInfo->ErrorID + " ");
        str.append(pRspInfo->ErrorMsg);
        str.append("\n");
        emit outputSignal(str);
    }
    return bResult;
}
