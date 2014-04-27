#include "dbmanager.h"

#include <QDebug>
#include <QMap>
#include "../ShareLib/share_headers/data_header/CommonStrut.h"

DbManager::DbManager()
{
}

bool DbManager::clearData()
{
    QString exeSql = QString("SELECT CONCAT( 'drop table ', table_name, ';' ) FROM information_schema.tables WHERE table_name LIKE 'tick_%';");

    //qDebug() << "clearData exeSql=" << exeSql;

    QSqlQuery query = db.exec(exeSql);

    while (query.next())
    {
        qDebug() << "query.value(0).toString()=" << query.value(0).toString();
        db.exec(query.value(0).toString());
    }

    return false;
}

bool DbManager::initDB(QString dbName)
{
    //db = QSqlDatabase::addDatabase("QMYSQL");
    db = QSqlDatabase::addDatabase("QSQLITE");
    //qDebug() << __FUNCTION__ <<"database drivers=" << db.drivers();

    //db.setHostName(ipAddr);
    //db.setPort(3306);
    db.setDatabaseName(dbName);
    //db.setUserName(UserName);
    //db.setPassword(Password);
    //db.setUserName("admin");
    //db.setPassword("password");
    if (!db.open())
    {
        qDebug() << "open database failed!" << db.lastError().text();
        return false;
    }

    return true;
}

void DbManager::resetData()
{
    currentInstruments.clear();
}

bool DbManager::insertTickData(TThostFtdcInstrumentIDType InstrumentID, TickDataStruct data)
{
    if (!currentInstruments.contains(InstrumentID))
    {
        currentInstruments.insert(InstrumentID, InstrumentID);

        QString createtable = QString("CREATE TABLE IF NOT EXISTS `tick_%1` (%2, %3, %4, %5, %6);")
                .arg(InstrumentID)
                .arg("`id`  INTEGER PRIMARY KEY AUTOINCREMENT")
                .arg("`date` DATE NOT NULL")
                .arg("`time` TIME NOT NULL")
                .arg("`LastPrice` DOUBLE NOT NULL")
                .arg("`volume` INT NOT NULL");
          //      .arg("ENGINE = InnoDB")
        db.exec(createtable);
        //qDebug() << "createtable : " << createtable;

    }

    QString exeSql = QString("INSERT INTO tick_%1 (date, time, LastPrice, volume) VALUES ('%2', '%3', '%4', '%5');")
            .arg(InstrumentID)
            .arg(QDate::currentDate().toString("yyyy-MM-dd"))
            .arg(data.UpdateTime)
            .arg(data.LastPrice)
            .arg(data.Volume);

    //qDebug() << "addTickData exeSql=" << exeSql;

    db.exec(exeSql);

    return true;
}

QList<TickDataStruct> DbManager::queryTickDataByIntrumentID(QString InstrumentID)
{
    QString exeSql = QString("select date, time, LastPrice, volume from tick_%1 order by time ASC")
            .arg(InstrumentID);

    QSqlQuery query = db.exec(exeSql);

    QList<TickDataStruct> result;

    while (query.next())
    {
        TickDataStruct data;
        data.date = query.value(0).toString();
        data.UpdateTime = query.value(1).toString();
        data.LastPrice = query.value(2).toDouble();
        data.Volume = query.value(3).toInt();
        result += data;
    }

    return result;
}

DbManager * DbManager::dbMgr = NULL;
