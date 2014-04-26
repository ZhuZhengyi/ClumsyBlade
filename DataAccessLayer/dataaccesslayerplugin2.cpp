#include "dataaccesslayerplugin2.h"
#include "../ShareLib/share_headers/utils/lessthan.h"
#include "dbmanager.h"

#include <QDebug>
#include <QDate>
#include <QMapIterator>

DataAccessLayerPlugin2::~DataAccessLayerPlugin2()
{
}

bool DataAccessLayerPlugin2::writeIndex(TThostFtdcInstrumentIDType IntrumentID, int ind)
{
    return true;
}

bool DataAccessLayerPlugin2::resetData()
{
    DbManager::getDBMgr()->resetData();
    return true;
}

bool DataAccessLayerPlugin2::initDB(QString dbName)
{
    //qDebug() << __FUNCTION__;
    return DbManager::getDBMgr()->initDB(dbName);
}

bool DataAccessLayerPlugin2::clearData()
{
    return DbManager::getDBMgr()->clearData();
}

bool DataAccessLayerPlugin2::closeDB()
{
    return DbManager::getDBMgr()->releaseDBMgr();
}

void DataAccessLayerPlugin2::insertTickData(TThostFtdcInstrumentIDType InstrumentID, TickDataStruct data)
{
    DbManager::getDBMgr()->insertTickData(InstrumentID, data);
}

QList<TickDataStruct> DataAccessLayerPlugin2::queryTickDataByIntrumentID(QString IntrumentID)
{
    qDebug() << "DataAccessLayerPlugin2 queryTickDataByIntrumentID" << IntrumentID;

    return DbManager::getDBMgr()->queryTickDataByIntrumentID(IntrumentID);

}

Q_EXPORT_PLUGIN2(DataAccessLayer, DataAccessLayerPlugin2)
