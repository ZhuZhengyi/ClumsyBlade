#ifndef COMMONSTRUT_H
#define COMMONSTRUT_H

#include "../ctp_headers/ThostFtdcUserApiDataType.h"

class QString;

struct TickDataStruct
{
    QString date;
    ///合约代码
    // TThostFtdcInstrumentIDType InstrumentID;
    ///最后修改时间
    QString UpdateTime;
    ///最新价
    TThostFtdcPriceType	LastPrice;
    ///数量
    TThostFtdcVolumeType Volume;
};

#endif // COMMONSTRUT_H
