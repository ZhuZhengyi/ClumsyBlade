#ifndef DATAACCESSLAYERPLUGIN2_H
#define DATAACCESSLAYERPLUGIN2_H

#include "dataaccesslayer.h"
#include <QMap>

typedef QMap<QString, long> IndexMap;

class DataAccessLayerPlugin2 : public QObject, DataAccessLayer
{
    Q_OBJECT
    Q_INTERFACES(DataAccessLayer)

private:

public:
    ~DataAccessLayerPlugin2();

    bool writeIndex(TThostFtdcInstrumentIDType, int);
    bool resetData();
    bool initDB(QString);
    bool clearData();
    bool closeDB();
    void insertTickData(TThostFtdcInstrumentIDType, TickDataStruct);
    QList<TickDataStruct> queryTickDataByIntrumentID(QString);
};

#endif // DATAACCESSLAYERPLUGIN_H
