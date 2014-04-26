#ifndef CTPMDWRAPPER_H
#define CTPMDWRAPPER_H

#include <QtGui>

#include "PVSConstant.h"
#include "ctpwrapper.h"
#include "../ShareLib/share_headers/ctp_headers/ThostFtdcMdApi.h"

class MdThread: public QThread
{
protected:
    void run();
};


class CTPMdWrapper : public CTPWrapper, public CThostFtdcMdSpi
{
    Q_OBJECT
public:
    CTPMdWrapper(QObject *parent = 0);
    ~CTPMdWrapper();

private:
    void initRes();
    void releaseRes();
    void ReqUserLogin();
    //
    bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);

    QStringList idList;

public:
    void FrontConnect();
    void SubscribeCurrentInstrumentInfo();

    virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnFrontDisconnected(int nReason);
    virtual void OnHeartBeatWarning(int nTimeLapse);
    virtual void OnFrontConnected();
    virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);

signals:
    void addInstrumentSignal(QString item, QString ExchangeID);
    void updateDataSignal(CThostFtdcDepthMarketDataField*);
    void onLoginSignal(bool);

public slots:
    void SubscribeInstrumentInfo(QString InstrumentID, QString ExchangeID);

    
};

#endif // CTPMDWRAPPER_H
