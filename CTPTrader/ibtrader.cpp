#include "ibtrader.h"
#include "ctpmdwrapper.h"
#include "ctptraderwrapper.h"

#include "UI/connectdlg.h"
#include "UI/aboutdlg.h"
#include "UI/strategyeditor.h"
#include "../ShareLib/share_headers/utils/datamanager.h"

#include <iostream>
using namespace std;

extern CThostFtdcMdApi* pMdApi;
extern CThostFtdcTraderApi* pTraderApi;

extern char FRONT_ADDR_MD[101];
extern char FRONT_ADDR_TRADER[101];

extern TThostFtdcBrokerIDType	BROKER_ID;				// 经纪公司代码
extern TThostFtdcInvestorIDType INVESTOR_ID;			// 投资者代码
extern TThostFtdcPasswordType  PASSWORD;			// 用户密码
extern int iRequestID;
extern int iTradeRequestID;                        //Added on July 2nd,

PVSniper::PVS_STATE state;


IBTrader::IBTrader(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
    initRes();
}

IBTrader::~IBTrader()
{
    DataManager::GetInstance()->unloadDAL();
    releaseRes();
}

void IBTrader::initRes()
{
    ui.setupUi(this);
    mdWrapper = new CTPMdWrapper(this);
    traderWrapper = new CTPTraderWrapper(this);

    setNewState(PVSniper::DISCONNECTED);

    ui.dockWidget_left->close();

    qRegisterMetaType<PVSniper::PVS_STATE>("PVSniper::PVS_STATE");

    connect(mdWrapper, SIGNAL(onLoginSignal(bool)), this, SLOT(onLoginMDSlot(bool)));
    connect(mdWrapper, SIGNAL(setNewStateSignal(PVSniper::PVS_STATE)), this, SLOT(setNewState(PVSniper::PVS_STATE)));
    connect(mdWrapper, SIGNAL(outputSignal(QString, bool)), this, SLOT(output2Log(QString, bool)));
    connect(mdWrapper, SIGNAL(updateDataSignal(CThostFtdcDepthMarketDataField *)), this, SLOT(updateData(CThostFtdcDepthMarketDataField *)));
    connect(mdWrapper, SIGNAL(addInstrumentSignal(QString, QString)), this, SLOT(addInstrument(QString, QString)));

    connect(traderWrapper, SIGNAL(onLoginSignal(bool)), this, SLOT(onLoginTraderSlot(bool)));
    connect(traderWrapper, SIGNAL(setNewStateSignal(PVSniper::PVS_STATE)), this, SLOT(setNewState(PVSniper::PVS_STATE)));
    connect(traderWrapper, SIGNAL(outputSignal(QString, bool)), this, SLOT(output2Log(QString, bool)));
    connect(traderWrapper, SIGNAL(addInstrumentSignal(QString, QString)), this, SLOT(addInstrument(QString, QString)));
    connect(traderWrapper, SIGNAL(onFrontDisconnectedSignal()), this, SLOT(onTraderDisconnectedSlot()));
    connect(traderWrapper, SIGNAL(onUserLogoutSignal()), this, SLOT(onTraderUserLogoutSlot()));
    connect(traderWrapper, SIGNAL(subscribeInstrumentInfoSignal(QString,QString)), mdWrapper, SLOT(SubscribeInstrumentInfo(QString,QString)));

    //menu
    connect(ui.actionConnect, SIGNAL(triggered()), this, SLOT(actionConnect()));
    connect(ui.actionDisconnect, SIGNAL(triggered()), this, SLOT(actionDisconnect()));
    connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(actionAbout()));
    connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(actionExit()));
    connect(ui.actionDemo, SIGNAL(triggered()), this, SLOT(actionDemo()));
    connect(ui.actionOrderInsert,SIGNAL(triggered()),this,SLOT(actionOrderInsert()));

    //Query
    connect(ui.actionQryExchange, SIGNAL(triggered()), this, SLOT(actionQryExchange()));
    connect(ui.actionQryInstrument, SIGNAL(triggered()), this, SLOT(actionQryInstrument()));
    connect(ui.actionQryInstrumentCommissionRate, SIGNAL(triggered()), this, SLOT(actionQryInstrumentCommissionRate()));
    connect(ui.actionQryInstrumentMarginRate, SIGNAL(triggered()), this, SLOT(actionQryInstrumentMarginRate()));

    //CapitalManagement
    connect(ui.actionBankToFuture, SIGNAL(triggered()), this, SLOT(actionBankToFuture()));

    connect(ui.actionLeft, SIGNAL(triggered(bool)), this, SLOT(actionLeftSlot(bool)));
    connect(ui.dockWidget_left, SIGNAL(visibilityChanged(bool)), this, SLOT(actionLeftSlot2(bool)));
    connect(ui.actionRight, SIGNAL(triggered(bool)), this, SLOT(actionRightSlot(bool)));
    connect(ui.dockWidget_right, SIGNAL(visibilityChanged(bool)), this, SLOT(actionRightSlot2(bool)));
    connect(ui.actionBottom, SIGNAL(triggered(bool)), this, SLOT(actionBottomSlot(bool)));
    connect(ui.dockWidget_bottom, SIGNAL(visibilityChanged(bool)), this, SLOT(actionBottomSlot2(bool)));

    connect(ui.clear_log, SIGNAL(clicked()), this, SLOT(clearLog()));

    connect(ui.shfeList, SIGNAL(updatePanelSignal(CThostFtdcDepthMarketDataField *)), ui.tickChart, SLOT(updateTick(CThostFtdcDepthMarketDataField *)));
    connect(ui.shfeList, SIGNAL(updatePanelSignal(CThostFtdcDepthMarketDataField *)), this, SLOT(updatePanel(CThostFtdcDepthMarketDataField *)));
    connect(ui.shfeList->getDelegate(), SIGNAL(switchToTickChartSignal(QString)), this, SLOT(switchToTickChart(QString)));
    connect(ui.shfeList, SIGNAL(addItemSignal(QStandardItem *)), ui.customList, SLOT(addItemSlot(QStandardItem *)));
    connect(ui.shfeList, SIGNAL(addModelSignal(QString)), ui.customList, SLOT(addModel(QString)));

    connect(ui.czceList, SIGNAL(updatePanelSignal(CThostFtdcDepthMarketDataField *)), ui.tickChart, SLOT(updateTick(CThostFtdcDepthMarketDataField *)));
    connect(ui.czceList, SIGNAL(updatePanelSignal(CThostFtdcDepthMarketDataField *)), this, SLOT(updatePanel(CThostFtdcDepthMarketDataField *)));
    connect(ui.czceList->getDelegate(), SIGNAL(switchToTickChartSignal(QString)), this, SLOT(switchToTickChart(QString)));
    connect(ui.czceList, SIGNAL(addItemSignal(QStandardItem *)), ui.customList, SLOT(addItemSlot(QStandardItem *)));

    connect(ui.dceList, SIGNAL(updatePanelSignal(CThostFtdcDepthMarketDataField *)), ui.tickChart, SLOT(updateTick(CThostFtdcDepthMarketDataField *)));
    connect(ui.dceList, SIGNAL(updatePanelSignal(CThostFtdcDepthMarketDataField *)), this, SLOT(updatePanel(CThostFtdcDepthMarketDataField *)));
    connect(ui.dceList->getDelegate(), SIGNAL(switchToTickChartSignal(QString)), this, SLOT(switchToTickChart(QString)));
    connect(ui.dceList, SIGNAL(addItemSignal(QStandardItem *)), ui.customList, SLOT(addItemSlot(QStandardItem *)));

    connect(ui.cffexList, SIGNAL(updatePanelSignal(CThostFtdcDepthMarketDataField *)), ui.tickChart, SLOT(updateTick(CThostFtdcDepthMarketDataField *)));
    connect(ui.cffexList, SIGNAL(updatePanelSignal(CThostFtdcDepthMarketDataField *)), this, SLOT(updatePanel(CThostFtdcDepthMarketDataField *)));
    connect(ui.cffexList->getDelegate(), SIGNAL(switchToTickChartSignal(QString)), this, SLOT(switchToTickChart(QString)));
    connect(ui.cffexList, SIGNAL(addItemSignal(QStandardItem *)), ui.customList, SLOT(addItemSlot(QStandardItem *)));

    connect(ui.customList->getDelegate(), SIGNAL(switchToTickChartSignal(QString)), this, SLOT(switchToTickChart(QString)));
}
void IBTrader::releaseRes()
{
    if (pMdApi)
    {
        pMdApi->RegisterSpi(NULL);
        pMdApi->Release();
        pMdApi = NULL;
    }

    if(pTraderApi)
    {
        pTraderApi->RegisterSpi(NULL);
        pTraderApi->Release();
        pTraderApi = NULL;
    }

    if (mdWrapper)
    {
        delete mdWrapper;
        mdWrapper = NULL;
    }

    if (traderWrapper)
    {
        delete traderWrapper;
        traderWrapper = NULL;
    }
}

void IBTrader::setNewState(PVSniper::PVS_STATE newState)
{
    state = newState;
    switch(state)
    {
    case PVSniper::DISCONNECTED:
        ui.actionConnect->setEnabled(true);
        ui.actionDisconnect->setEnabled(false);
        ui.actionOrderInsert->setEnabled(false);
        ui.actionQryExchange->setEnabled(false);
        ui.menuBaseInfoQry->setEnabled(false);
        break;
    case PVSniper::CONNECTED:
        ui.actionConnect->setEnabled(false);
        ui.actionDisconnect->setEnabled(true);
        ui.actionOrderInsert->setEnabled(true);
        ui.actionQryExchange->setEnabled(true);
        ui.actionQryInvestor->setEnabled(true);
        ui.menuBaseInfoQry->setEnabled(true);
        break;
    case PVSniper::ITEM_SELECTED:
        break;
    case PVSniper::IN_PROCESS:
        break;
    }
}
void IBTrader::output2Log(QString str, bool append)
{
    if(append) {
        ui.log_output->append(str);
    }else{
        ui.log_output->setText(str);
    }
}
void IBTrader::clearLog()
{
    ui.log_output->clear();
}
void IBTrader::switchToTickChart(QString InstrumentID)
{
    ui.tickChart->clearAllData(InstrumentID);
    ui.tabWidget_main->setCurrentIndex(0);
}

void IBTrader::updateData(CThostFtdcDepthMarketDataField *data)
{
    //qDebug() << "data->ExchangeID=" << data->ExchangeID;
    if(!QString::compare(data->ExchangeID, "CFFEX", Qt::CaseInsensitive))
    {
        //qDebug() << "data->ExchangeID=" << "CFFEX";
        ui.cffexList->updateData(data);
    }
    else if(!QString::compare(data->ExchangeID, "CZCE", Qt::CaseInsensitive))
    {
        //qDebug() << "data->ExchangeID=" << "CZCE";
        ui.czceList->updateData(data);
    }
    else if(!QString::compare(data->ExchangeID, "DCE", Qt::CaseInsensitive))
    {
        //qDebug() << "data->ExchangeID=" << "DCE";
        ui.dceList->updateData(data);
    }
    else
    {
        //qDebug() << "data->ExchangeID=" << "SHFE";
        ui.shfeList->updateData(data);
    }

    ui.customList->updateData(data);

    //updatePanel(data);
}
void IBTrader::updatePanel(CThostFtdcDepthMarketDataField *data)
{
    ui.InstrumentID_val->setText(data->InstrumentID);
    ui.LastPrice_val->setText(checkValue(data->LastPrice));
    ui.LastPrice_val->setStyleSheet(getStyleSheetByVal(data->LastPrice, data->PreClosePrice));
    ui.AskPrice1_val->setText(checkValue(data->AskPrice1));
    ui.AskPrice1_val->setStyleSheet(getStyleSheetByVal(data->AskPrice1, data->PreClosePrice));
    ui.AskPrice2_val->setText(checkValue(data->AskPrice2));
    ui.AskPrice2_val->setStyleSheet(getStyleSheetByVal(data->AskPrice2, data->PreClosePrice));
    ui.AskPrice3_val->setText(checkValue(data->AskPrice3));
    ui.AskPrice3_val->setStyleSheet(getStyleSheetByVal(data->AskPrice3, data->PreClosePrice));
    ui.AskPrice4_val->setText(checkValue(data->AskPrice4));
    ui.AskPrice4_val->setStyleSheet(getStyleSheetByVal(data->AskPrice4, data->PreClosePrice));
    ui.AskPrice5_val->setText(checkValue(data->AskPrice5));
    ui.AskPrice5_val->setStyleSheet(getStyleSheetByVal(data->AskPrice5, data->PreClosePrice));
    ui.BidPrice1_val->setText(checkValue(data->BidPrice1));
    ui.BidPrice1_val->setStyleSheet(getStyleSheetByVal(data->BidPrice1, data->PreClosePrice));
    ui.BidPrice2_val->setText(checkValue(data->BidPrice2));
    ui.BidPrice2_val->setStyleSheet(getStyleSheetByVal(data->BidPrice2, data->PreClosePrice));
    ui.BidPrice3_val->setText(checkValue(data->BidPrice3));
    ui.BidPrice3_val->setStyleSheet(getStyleSheetByVal(data->BidPrice3, data->PreClosePrice));
    ui.BidPrice4_val->setText(checkValue(data->BidPrice4));
    ui.BidPrice4_val->setStyleSheet(getStyleSheetByVal(data->BidPrice4, data->PreClosePrice));
    ui.BidPrice5_val->setText(checkValue(data->BidPrice5));
    ui.BidPrice5_val->setStyleSheet(getStyleSheetByVal(data->BidPrice5, data->PreClosePrice));
}

void IBTrader::onLoginMDSlot(bool login)
{
    if(login){
        traderWrapper->FrontConnect();
    }
}
void IBTrader::onLoginTraderSlot(bool login)
{
    if(login){
        setNewState(PVSniper::CONNECTED);
    }
}
void IBTrader::onTraderDisconnectedSlot()
{
    setNewState(PVSniper::DISCONNECTED);
    output2Log(tr("trader disconnected"));
}
void IBTrader::onTraderUserLogoutSlot()
{
    output2Log(tr("trader logout"));
}

void IBTrader::actionConnect()
{
    ConnectDlg dlg;
    switch(dlg.exec())
    {
    case QDialog::Accepted:
    {
        memset(FRONT_ADDR_MD, 0, 101);
        memset(FRONT_ADDR_TRADER, 0, 101);
        memset(BROKER_ID, 0, 11);
        memset(INVESTOR_ID, 0, 13);
        memset(PASSWORD, 0, 41);

        strcpy(FRONT_ADDR_MD, dlg.getMdAddr().toLatin1().data());
        strcpy(FRONT_ADDR_TRADER, dlg.getTradingAddr().toLatin1().data());
        strcpy(BROKER_ID, dlg.getBrokerId().toLatin1().data());
        strcpy(INVESTOR_ID, dlg.getUserName().toLatin1().data());
        strcpy(PASSWORD, dlg.getPwd().toLatin1().data());

        mdWrapper->FrontConnect();

        setNewState(PVSniper::PVS_STATE::IN_PROCESS);


        break;
    }
    case QDialog::Rejected:
    {
        break;
    }
    }
}
void IBTrader::actionDisconnect()
{
    CThostFtdcUserLogoutField field;
    memset(&field, 0 , sizeof(field));
    strcpy(field.BrokerID, BROKER_ID);


    if(pTraderApi){
        pTraderApi->ReqUserLogout(&field,  ++iRequestID);
    }

    if(pMdApi){
        pMdApi->ReqUserLogout(&field, ++iRequestID);
    }

}
void IBTrader::actionDemo()
{
    StrategyEditor se(LAN_LUA);
    se.exec();
}
void IBTrader::actionQryInstrumentCommissionRate()
{
    CThostFtdcQryInstrumentCommissionRateField field;
    memset(&field, 0, sizeof(field));
    strcpy(field.InvestorID, INVESTOR_ID);
    pTraderApi->ReqQryInstrumentCommissionRate(&field, ++iRequestID);
}
void IBTrader::actionQryInstrumentMarginRate()
{
    CThostFtdcQryInstrumentMarginRateField field;
    memset(&field, 0, sizeof(field));
    strcpy(field.InvestorID, INVESTOR_ID);
    pTraderApi->ReqQryInstrumentMarginRate(&field, ++iRequestID);
}
void IBTrader::actionBankToFuture()
{
    CThostFtdcReqTransferField field;
    memset(&field, 0, sizeof(field));
    strcpy(field.AccountID, INVESTOR_ID);
    //strcpy(field.BankAccount, "6222600110030801158");
    pTraderApi->ReqFromBankToFutureByFuture(&field, ++iRequestID);
}
void IBTrader::actionQryInstrument()
{
    CThostFtdcQryInstrumentField field;
    memset(&field, 0, sizeof(field));
    strcpy(field.InstrumentID, "cu1207");
    if(pTraderApi){
        pTraderApi->ReqQryInstrument(&field, ++iRequestID);
    }
}
void IBTrader::actionAbout()
{
    AboutDlg aDlg;
    aDlg.exec();
}
void IBTrader::actionLeftSlot(bool b)
{
    ui.dockWidget_left->setVisible(b);
}
void IBTrader::actionLeftSlot2(bool b)
{
    ui.actionLeft->setChecked(b);
}
void IBTrader::actionRightSlot(bool b)
{
    ui.dockWidget_right->setVisible(b);
}
void IBTrader::actionRightSlot2(bool b)
{
    ui.actionRight->setChecked(b);
}
void IBTrader::actionBottomSlot(bool b)
{
    ui.dockWidget_bottom->setVisible(b);
}
void IBTrader::actionBottomSlot2(bool b)
{
    ui.actionBottom->setChecked(b);
}
void IBTrader::addInstrument(QString item, QString ExchangeID)
{
    if(!QString::compare(ExchangeID, "CFFEX", Qt::CaseInsensitive))
    {
        ui.cffexList->addModel(item);
    }
    else if(!QString::compare(ExchangeID, "CZCE", Qt::CaseInsensitive))
    {
        ui.czceList->addModel(item);
    }
    else if(!QString::compare(ExchangeID, "DCE", Qt::CaseInsensitive))
    {
        ui.dceList->addModel(item);
    }
    else
    {
        ui.shfeList->addModel(item);
    }
}
void IBTrader::actionOrderInsert()
{
    CThostFtdcQryTradingAccountField reqAc;
    memset(&reqAc, 0, sizeof(reqAc));
    strcpy(reqAc.BrokerID, BROKER_ID);
    strcpy(reqAc.InvestorID, INVESTOR_ID);
    int iResult = pTraderApi->ReqQryTradingAccount(&reqAc, ++iRequestID);
    qDebug() << "--->>> 请求查询资金账户: " << ((iResult == 0) ? "成功" : "失败") << iResult << endl;

    CThostFtdcQrySettlementInfoField reqInfo;
    memset(&reqInfo, 0, sizeof(reqInfo));
    strcpy(reqInfo.BrokerID ,BROKER_ID);
    strcpy(reqInfo.InvestorID,INVESTOR_ID);
    pTraderApi->ReqQrySettlementInfo(&reqInfo, ++iRequestID);

    CThostFtdcSettlementInfoConfirmField req;
    memset(&req, 0, sizeof(req));
    strcpy(req.BrokerID, BROKER_ID);
    strcpy(req.InvestorID, INVESTOR_ID);
    iResult = pTraderApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
    qDebug() << "--->>> 投资者结算结果确认: " << ((iResult == 0) ? "成功" : "失败") << endl;

    qDebug() << "IBTrader::actionOrderInsert";
    ///**************************
    CThostFtdcInputOrderField ord;
    memset(&ord, 0, sizeof(ord));
    //经纪公司代码
    strcpy(ord.BrokerID, BROKER_ID);
    //投资者代码
    strcpy(ord.InvestorID, INVESTOR_ID);
    //合约代码
    strcpy(ord.InstrumentID,"cu1207");
    //报单引用
    strcpy(ord.OrderRef,"5");
    //用户代码
    //strcpy(ord.UserID, INVESTOR_ID);
    //报单价格条件
    ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
    //买卖方向
    ord.Direction = THOST_FTDC_D_Buy;
    //组合开平标志
    strcpy(ord.CombOffsetFlag,"0");
    //组合投机套保标志
    strcpy(ord.CombHedgeFlag,"1");
    //价格
    ord.LimitPrice = 56400;
    //数量
    ord.VolumeTotalOriginal = 10;
    //有效期类型
    ord.TimeCondition = THOST_FTDC_TC_GFD;
    //GTD日期
    strcpy(ord.GTDDate,"");
    //成交量类型
    ord.VolumeCondition = THOST_FTDC_VC_AV;
    //最小成交量
    ord.MinVolume =1;
    //触发条件
    ord.ContingentCondition = THOST_FTDC_CC_Immediately;
    //止损价
    ord.StopPrice = 0;
    //强平原因
    ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
    //自动挂起标志
    ord.IsAutoSuspend =1;
    ord.UserForceClose=0;
    //    *******************/
    //
    // add the interface to set data req, provided by trading strategy.
    //InitializeInputOrderField(), to be coded
    pTraderApi->ReqOrderInsert(&ord, ++iRequestID+2);
}
void IBTrader::actionExit()
{
    exit(0);
}
void IBTrader::actionQryExchange()
{
    //qDebug() << __FUNCTION__ ;
    CThostFtdcQryExchangeField field;
    memset(&field, 0, sizeof(field));
    if (pTraderApi){
        pTraderApi->ReqQryExchange(&field, iRequestID++);
    }
}

