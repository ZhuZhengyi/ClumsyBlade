#ifndef CTP_DATASTRUCT_H
#define CTP_DATASTRUCT_H

#include "../ctp_headers/ThostFtdcUserApiDataType.h"

struct etiger_MdUserDataField
{
    void *pMdUserApi;
    void *pMdUserSpi;
    TThostFtdcBrokerIDType BrokerID;
    TThostFtdcUserIDType UserID;
    TThostFtdcInvestorIDType InvestorID;
    TThostFtdcPasswordType Password;
    char** ppInstrumentID;
    int iInstrumentID;
    int iRequestID;
};

/* CMdUserSpi class */
void *ezctp_md_CreateCMdUserSpi(struct ezctp_MdUserDataField *pMdUserData);
void ezctp_md_ReleaseCMdUserSpi(void *thisC);

/* CThostFtdcMdApi class */
void *etiger_md_CreateFtdcMdApi(const char *pszFlowPath, const bool bIsUsingUdp);
void etiger_md_Release(void *thisC);
void etiger_md_Init(void *thisC);
int etiger_md_Join(void *thisC);
const char *etiger_md_GetTradingDay(void *thisC);
void etiger_md_RegisterFront(void *thisC, char *pszFrontAddress);
void etiger_md_RegisterSpi(void *thisC, void *pSpi);
int etiger_md_SubscribeMarketData(void *thisC, char *ppInstrumentID[], int nCount);
int etiger_md_UnSubscribeMarketData(void *thisC, char *ppInstrumentID[], int nCount);
int etiger_md_ReqUserLogin(void *thisC, struct CThostFtdcReqUserLoginField *pReqUserLoginField, int nRequestID);
int etiger_md_ReqUserLogout(void *thisC, struct CThostFtdcUserLogoutField *pUserLogout, int nRequestID);


#endif // CTP_DATASTRUCT_H
