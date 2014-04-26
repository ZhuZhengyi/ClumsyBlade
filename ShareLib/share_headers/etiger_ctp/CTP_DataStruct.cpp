#include "CTP_DataStruct.h"
#include "../ctp_headers/ThostFtdcMdApi.h"

#include <QDebug>

/********************
 * FtdcMdApi
 ********************/
void * etiger_md_CreateFtdcMdApi(const char * pszFlowPath, const bool bIsUsingUdp)
{
    qDebug() << Q_FUNC_INFO << "pszFlowPath=" << pszFlowPath;
    return CThostFtdcMdApi::CreateFtdcMdApi(pszFlowPath, bIsUsingUdp);
}

void etiger_md_Release(void *thisC)
{
    qDebug() << Q_FUNC_INFO << (qulonglong) thisC;
    static_cast<CThostFtdcMdApi*>(thisC)->Release();
}

void etiger_md_Init(void *thisC)
{
    qDebug() << Q_FUNC_INFO << (qulonglong) thisC;
    static_cast<CThostFtdcMdApi*>(thisC)->Init();
}

int etiger_md_Join(void *thisC)
{
    qDebug() << Q_FUNC_INFO << (qulonglong) thisC;
    return static_cast<CThostFtdcMdApi*>(thisC)->Join();
}

const char *etiger_md_GetTradingDay(void *thisC)
{
    return static_cast<CThostFtdcMdApi*>(thisC)->GetTradingDay();
}

void etiger_md_RegisterFront(void *thisC, char *pszFrontAddress)
{
    qDebug() << Q_FUNC_INFO << " mdapi=" << (qulonglong) thisC << "pszFrontAddress=" << pszFrontAddress;
    static_cast<CThostFtdcMdApi*>(thisC)->RegisterFront(pszFrontAddress);
}

void etiger_md_RegisterSpi(void *thisC, void *pSpi)
{
    qDebug() << Q_FUNC_INFO << " mdapi=" << (qulonglong) thisC;
    static_cast<CThostFtdcMdApi*>(thisC)->RegisterSpi(static_cast<CThostFtdcMdSpi*>(pSpi));
}

int etiger_md_SubscribeMarketData(void *thisC, char *ppInstrumentID[], int nCount)
{
    return static_cast<CThostFtdcMdApi*>(thisC)->SubscribeMarketData(ppInstrumentID, nCount);
}

int etiger_md_UnSubscribeMarketData(void *thisC, char *ppInstrumentID[], int nCount)
{
    return static_cast<CThostFtdcMdApi*>(thisC)->UnSubscribeMarketData(ppInstrumentID, nCount);
}

int etiger_md_ReqUserLogin(void *thisC, CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
{
    return static_cast<CThostFtdcMdApi*>(thisC)->ReqUserLogin(pReqUserLoginField, nRequestID);
}

int etiger_md_ReqUserLogout(void *thisC, CThostFtdcUserLogoutField *pUserLogout, int nRequestID)
{
    return static_cast<CThostFtdcMdApi*>(thisC)->ReqUserLogout(pUserLogout, nRequestID);
}
