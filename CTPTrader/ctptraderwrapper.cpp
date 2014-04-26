#include "ctptraderwrapper.h"
#include "ctpmdwrapper.h"

#include "../ShareLib/share_headers/utils/datamanager.h"
#include "../DataAccessLayer/dataaccesslayer.h"
#include "../ShareLib/share_headers/utils/filesystemhelper.h"
#include "PVSConstant.h"

// 配置参数
//int iTradeRequestID;          //Added on July 2nd, the request ID used in ReqOrderInsert().
char FRONT_ADDR_TRADER[101];		// 前置地址
extern TThostFtdcBrokerIDType	BROKER_ID;				// 经纪公司代码
extern TThostFtdcInvestorIDType INVESTOR_ID;			// 投资者代码
extern TThostFtdcPasswordType  PASSWORD;			// 用户密码
extern int iRequestID;

CThostFtdcTraderApi* pTraderApi;
extern CThostFtdcMdApi* pMdApi;

//QList<QString> idList;
int counter;

void TraderThread::run()
{
    if(!pTraderApi)
    {
        qDebug() << __FUNCTION__ << "pTraderApi is null";
        return;
    }

    pTraderApi->SubscribePrivateTopic(THOST_TERT_QUICK);
    pTraderApi->SubscribePublicTopic(THOST_TERT_QUICK);
    pTraderApi->RegisterFront(FRONT_ADDR_TRADER);
    pTraderApi->Init();
    pTraderApi->Join();
}

CTPTraderWrapper::CTPTraderWrapper(QObject *parent)
    :CTPWrapper(parent)
{
    // t = NULL;
    initRes();
}

CTPTraderWrapper::~CTPTraderWrapper()
{
    idList.clear();
    releaseRes();
}

void CTPTraderWrapper::initRes()
{
    createDirInNotExist(PVSniper::TRADER_CON);

    pTraderApi = CThostFtdcTraderApi::CreateFtdcTraderApi(PVSniper::TRADER_CON.toAscii().constData());

    if(!pTraderApi){
        qDebug() << "initTraderSlot failed!";
        return;
    }

    pTraderApi->RegisterSpi(this);				// 注册事件类
}

void CTPTraderWrapper::releaseRes()
{
    if(pTraderApi)
    {
        pTraderApi->RegisterSpi(NULL);
        pTraderApi->Release();
        pTraderApi = NULL;
    }

}

void CTPTraderWrapper::FrontConnect()
{
    TraderThread *t=new TraderThread();
    t->start();

    emit outputSignal(tr("connecting to trade server..."), false);
}

void CTPTraderWrapper::OnFrontDisconnected(int nReason)
{
    qDebug() << __FUNCTION__ << " nReason" << nReason;
    emit onFrontDisconnectedSignal();
}

bool CTPTraderWrapper::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
    // 如果ErrorID != 0, 说明收到了错误的响应
    bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
    if (bResult)
    {
        QString str = QString();
        str.append(pRspInfo->ErrorID + " ");
        str.append(pRspInfo->ErrorMsg);
        qDebug() << __FUNCTION__ << str;
        //emit outputSignal(str);
    }
    return bResult;
}

void CTPTraderWrapper::OnFrontConnected()
{
    emit outputSignal(tr("connect to trade server succeed!"), false);

    CThostFtdcReqUserLoginField req;
    memset(&req, 0, sizeof(req));

    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.UserID, INVESTOR_ID);
    strcpy(req.Password, PASSWORD);

    pTraderApi->ReqUserLogin(&req, ++iRequestID);
    emit outputSignal(tr("login to trade server..."), false);
    //emit OnTraderFrontConnectedSignal();
}

void CTPTraderWrapper::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

    if (!bIsLast || IsErrorRspInfo(pRspInfo)){
        qDebug() << __FUNCTION__ << bIsLast << pRspInfo->ErrorMsg;

        emit outputSignal( pRspInfo->ErrorMsg);
        emit onLoginSignal(false);
        return;
    }

    emit outputSignal(tr("login to trade server succeed!"), false);
    emit onLoginSignal(true);

    CThostFtdcQryInstrumentField req2;
    memset(&req2, 0, sizeof(req2));
    DataManager::GetInstance()->getDAL()->resetData();
    idList.clear();
    counter = 0;

    pTraderApi->ReqQryInstrument(&req2, ++iRequestID);

}

///请求查询合约响应
void CTPTraderWrapper::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    emit subscribeInstrumentInfoSignal(QString(pInstrument->InstrumentID), QString(pInstrument->ExchangeID));
    emit addInstrumentSignal(QString(pInstrument->InstrumentID), QString(pInstrument->ExchangeID));
}

void CTPTraderWrapper::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    qDebug() << __FUNCTION__;
    qDebug() << pRspInfo->ErrorMsg;
    qDebug()<<"报单未通过参数校验,被CTP拒绝";
}

void CTPTraderWrapper::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
    qDebug() << __FUNCTION__;
    qDebug() << pOrder->OrderStatus;
    qDebug()<<pOrder->OrderSysID;
    if('a'==pOrder->OrderStatus)
        qDebug()<<"CTP接受Order，但未发到交易所";

    /*
#define THOST_FTDC_OST_AllTraded '0'
///部分成交还在队列中
#define THOST_FTDC_OST_PartTradedQueueing '1'
///部分成交不在队列中
#define THOST_FTDC_OST_PartTradedNotQueueing '2'
///未成交还在队列中
#define THOST_FTDC_OST_NoTradeQueueing '3'
///未成交不在队列中
#define THOST_FTDC_OST_NoTradeNotQueueing '4'
///撤单
#define THOST_FTDC_OST_Canceled '5'
///未知
#define THOST_FTDC_OST_Unknown 'a'
///尚未触发
#define THOST_FTDC_OST_NotTouched 'b'
///已触发
#define THOST_FTDC_OST_Touched 'c'

typedef char TThostFtdcOrderStatusType;
      */


        //如果Thost接受了报单指令，用户不会收到OnRspOrderInser，而会收到OnRtnOrder，用来更新委托状态。

}

void CTPTraderWrapper::OnErrRtnOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
    qDebug()<<" 交易所报单录入错误回报";
}

void CTPTraderWrapper::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
  qDebug() << __FUNCTION__;
  qDebug()<<"成交通知";

  //交易所收到报单后，通过校验。用户会收到OnRtnOrder、OnRtnTrade。
}

void CTPTraderWrapper::OnErrRtnOrder(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo)
{
     qDebug() << __FUNCTION__;
     qDebug() << pRspInfo->ErrorMsg;
//如果交易所认为报单错误，用户就会收到OnErrRtnOrder。

}
//*********************************************
//*********************************************
//*********************************************

 void CTPTraderWrapper::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField *pSettlementInfo, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    qDebug()<<__FUNCTION__;
}



void CTPTraderWrapper::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    qDebug() << __FUNCTION__;
}

void CTPTraderWrapper::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
    qDebug() << __FUNCTION__;
    if(bIsLast&&!IsErrorRspInfo(pRspInfo))
    {
        CThostFtdcQryInvestorPositionField req;
        memset(&req, 0, sizeof(req));
        strcpy(req.BrokerID, BROKER_ID);
        strcpy(req.InvestorID, INVESTOR_ID);

        int iResult = pTraderApi->ReqQryInvestorPosition(&req, ++iRequestID);
        qDebug() << "--->>> 请求查询投资者持仓: " << ((iResult == 0) ? "成功" : "失败") << endl;

    }
}

///请求查询合约保证金率响应
void CTPTraderWrapper::OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

///请求查询合约手续费率响应
void CTPTraderWrapper::OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

///请求查询交易所响应
void CTPTraderWrapper::OnRspQryExchange(CThostFtdcExchangeField *pExchange, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
   // qDebug() << "pExchange ExchangeID=" << pExchange->ExchangeID;
   // qDebug() << "pExchange ExchangeName=" << pExchange->ExchangeName;
   // qDebug() << "pExchange ExchangeProperty=" << pExchange->ExchangeProperty;

    QString str = QString();
    str.append(pExchange->ExchangeID);
    str.append(pExchange->ExchangeName);
    str.append(pExchange->ExchangeProperty);

    qDebug() << __FUNCTION__ << str;

    emit outputSignal(str);
}

