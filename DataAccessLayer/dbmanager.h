#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QtSql>
#include <QtCore>

#include "../ShareLib/share_headers/data_header/CommonStrut.h"

class DbManager
{
private:
    DbManager();
    ~DbManager()
    {
        releaseDBMgr();
        if (dbMgr)
        {
            delete dbMgr;
            dbMgr = NULL;
        }
    }

    QSqlDatabase db;
    static DbManager *dbMgr;

    QMap<QString, QString> currentInstruments;

public:

    bool clearData();
    bool initDB(QString);
    void resetData();

    static DbManager * getDBMgr()
    {
        if(!dbMgr)
            dbMgr = new DbManager;
        return dbMgr;
    }

    static bool releaseDBMgr()
    {
        if (dbMgr->db.isOpen())
        {
            dbMgr->db.close();            
            return true;
        }

        return false;
    }

    bool insertTickData(TThostFtdcInstrumentIDType InstrumentID, TickDataStruct data);

    QList<TickDataStruct> queryTickDataByIntrumentID(QString InstrumentID);

};

#endif // DBMANAGER_H
