#ifndef DATAACCESSLAYER_H
#define DATAACCESSLAYER_H

#include <QtCore>
#include "../ShareLib/share_headers/data_header/CommonStrut.h"

struct TickDataStruct;

class DataAccessLayer
{
public:
    virtual bool writeIndex(TThostFtdcInstrumentIDType, int) = 0;
    virtual bool resetData() = 0;
    virtual bool initDB(QString) = 0;
    virtual bool closeDB() = 0;
    virtual bool clearData() = 0;
    virtual void insertTickData(TThostFtdcInstrumentIDType, TickDataStruct) = 0;
    virtual QList<TickDataStruct> queryTickDataByIntrumentID(QString) = 0;

};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(DataAccessLayer, "DataAccessLayerDLL/1.0")
QT_END_NAMESPACE

#endif // DATAACCESSLAYER_H
