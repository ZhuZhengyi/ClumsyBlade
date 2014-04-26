#ifndef DATAACCESSLAYERPLUGIN_H
#define DATAACCESSLAYERPLUGIN_H

#include "dataaccesslayer.h"
#include <QMap>

typedef QMap<QString, long> IndexMap;

class DataAccessLayerPlugin : public QObject, DataAccessLayer
{
    Q_OBJECT
    Q_INTERFACES(DataAccessLayer)

private:
    QMap<QString, IndexMap *> * historyMap;
    IndexMap currentMap;
    void writeMap2IndexFile();

public:
    ~DataAccessLayerPlugin();

    bool writeIndex(TThostFtdcInstrumentIDType, int);
    bool resetData();
    bool initDB(QString);
    bool closeDB();
    void insertTickData(TThostFtdcInstrumentIDType, TickDataStruct);
    QList<TickDataStruct> queryTickDataByIntrumentID(QString IntrumentID);
};

#endif // DATAACCESSLAYERPLUGIN_H
