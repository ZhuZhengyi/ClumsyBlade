#include "dataaccesslayerplugin.h"
#include "../ShareLib/share_headers/utils/lessthan.h"

#include <QDebug>
#include <QDate>
#include <QMapIterator>

const int step = 27000;
const int size = 21;
const int block_size = size * step;

const QString indexFile = ".ind";
const QString dataFile = ".dat";
const QString dateFormat = "yyyyMMdd";
const QString fileFormat = "data/%1/%2%3";
const QString index_dir = "index_sector";
const QString data_dir = "data_sector";

DataAccessLayerPlugin::~DataAccessLayerPlugin()
{
}


void DataAccessLayerPlugin::writeMap2IndexFile()
{
    QDate d = QDate::currentDate();
    QString fileName = QString(fileFormat).arg(index_dir).arg(d.toString(dateFormat)).arg(indexFile);

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);

    QTextStream ds(&file);
    QMapIterator<QString, long> i(currentMap);
    while (i.hasNext())
    {
        QMap<QString, long>::const_iterator item = i.next();
        ds << item.key() << "=" << item.value() << "\r\n";
    }

    file.close();

}

bool DataAccessLayerPlugin::writeIndex(TThostFtdcInstrumentIDType IntrumentID, int ind)
{

    QDate d = QDate::currentDate();
    QString fileName = QString(fileFormat).arg(index_dir).arg(d.toString(dateFormat)).arg(indexFile);

    QFile file(fileName);
    file.open(QIODevice::Append);

    QTextStream ds(&file);
    // ds.setVersion(QDataStream::Qt_4_8);
    long offset = ind * block_size;
    qDebug() << "writeIndex " << " ind=" << ind << " block_size=" << block_size << " offset=" << offset;
    ds << IntrumentID << "=" << offset << "\r\n";

    currentMap.insert(IntrumentID, -1);

    file.close();

    return true;
}

bool DataAccessLayerPlugin::resetData()
{
    currentMap.clear();

    QDate d = QDate::currentDate();
    QString fileName = QString(fileFormat).arg(index_dir).arg(d.toString(dateFormat)).arg(indexFile);

    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.close();

    QString fileName2 = QString(fileFormat).arg(data_dir).arg(d.toString(dateFormat)).arg(dataFile);
    QFile file2(fileName2);
    file2.open(QIODevice::WriteOnly);
    file2.close();

    return true;
}

bool DataAccessLayerPlugin::initDB(QString dbname)
{
    qDebug() << __FUNCTION__;
    historyMap = new QMap<QString, IndexMap *>;

    QDir indexDir("data/" + index_dir);

    indexDir.setFilter(QDir::Files);
    QStringList filters;
    filters << "*.ind";
    indexDir.setNameFilters(filters);
    QFileInfoList infoList = indexDir.entryInfoList();
    qSort(infoList.begin(), infoList.end(), FileNameLessThan());

    Q_FOREACH (QFileInfo info, infoList)
    {
        qDebug() << info.absoluteFilePath();
        QFile f(info.absoluteFilePath());
        f.open(QIODevice::ReadOnly);
        QTextStream ts(&f);
        QString res = ts.readAll();
        QStringList mapDatas = res.trimmed().split("\r\n");
        IndexMap * map = new IndexMap;
        Q_FOREACH (QString mapData, mapDatas)
        {
            QStringList mapDataList = mapData.trimmed().split("=");
            map->insert(mapDataList.at(0), mapDataList.at(1).toLong());
        }

        historyMap->insert(info.baseName(), map);

        f.close();
    }

    qDebug() << *historyMap;
    return true;
}

bool DataAccessLayerPlugin::closeDB()
{
    if (historyMap)
    {
        QMapIterator<QString, IndexMap *> i(* historyMap);
        while (i.hasNext())
        {
            QMap<QString, IndexMap *>::const_iterator item = i.next();
            IndexMap * val = item.value();
            // qDebug() << *val;
            val->clear();
            delete val;
            val = NULL;
        }

        delete historyMap;

        historyMap = NULL;

    }
    return false;
    //return DbManager::getDBMgr()->releaseDBMgr();
}

void DataAccessLayerPlugin::insertTickData(TThostFtdcInstrumentIDType InstrumentID, TickDataStruct data)
{

    QDate d = QDate::currentDate();
    QString fileName = QString(fileFormat).arg(data_dir).arg(d.toString(dateFormat)).arg(dataFile);

    QFile file(fileName);

    if (!file.open(QIODevice::Append))
        return;

    QDataStream ds(&file);

    ds.setVersion(QDataStream::Qt_4_8);
    // qDebug() << "sizeof=" << sizeof(data.UpdateTime) + sizeof(data.LastPrice) + sizeof(data.Volume);
    long offset = currentMap.value(InstrumentID, -1);
    qDebug() << "data.InstrumentID=" << InstrumentID << " offset=" << offset;
    if (offset == -1)
    {
        int ind = currentMap.keys().indexOf(InstrumentID);
        if (ind >= 0)
        {
            offset = ind * block_size;
        }
    }
    if (offset != -1)
    {
        file.seek(offset);
    }

    qDebug() << "insertTickData offset=" << offset;
    qDebug() << "sizeofdata=" << sizeof(data);
    qDebug() << "sizeofdata 2=" << sizeof(data.UpdateTime) + sizeof(data.LastPrice) + sizeof(data.Volume);
    ds << QString(data.UpdateTime) << (qreal)data.LastPrice << (qreal)data.Volume;

    currentMap.insert(InstrumentID, offset + size);

    file.close();

    writeMap2IndexFile();

    //DbManager::getDBMgr()->insertTickData(data);
}

QList<TickDataStruct> DataAccessLayerPlugin::queryTickDataByIntrumentID(QString IntrumentID)
{
    qDebug() << "DataAccessLayerPlugin queryTickDataByIntrumentID" << IntrumentID;

    QList<TickDataStruct> result;

    if (historyMap)
    {
        QMapIterator<QString, IndexMap *> i(* historyMap);
        while (i.hasNext())
        {
            // key -> date .value -> hashmap
            QMap<QString, IndexMap *>::const_iterator item = i.next();
            // key -> instrumentId value -> maxoffset
            IndexMap * val = item.value();
            int ind = val->keys().indexOf(IntrumentID);
            // find the instrument within the day.
            if (ind >= 0)
            {
                long beginOffset = ind * block_size;
                long maxOffset = val->value(IntrumentID);
                if (maxOffset == -1)
                    continue;
                int num = (maxOffset - beginOffset) / size;
                int left = (maxOffset - beginOffset) % size;
                qDebug() << "date=" << i.key() << " IntrumentId=" << IntrumentID
                         << "beginOffset=" << beginOffset << " maxOffset=" << maxOffset
                         << " ind=" << ind << " num=" << num << " left=" << left;

                QString fileName = QString(fileFormat).arg(data_dir).arg(i.key()).arg(dataFile);

                QFile df(fileName);

                qDebug() << "df=" << df.fileName()<< " isOpen=" << df.isOpen() << " exists=" << df.exists();

                if (!df.open(QIODevice::ReadOnly))
                    continue;

                qDebug() << "df opened=" << df.fileName();

                QDataStream ds(&df);
                ds.setVersion(QDataStream::Qt_4_8);

                df.seek(beginOffset);

                for (int i = 0; i < num; i++)
                {
                    TickDataStruct data;
                    QString UpdateTime;
                    ds >> UpdateTime >> data.LastPrice >> data.Volume;
                    // strcpy(data.UpdateTime, UpdateTime.toLatin1().constData());

                    qDebug() << " updatetime=" << data.UpdateTime << " LastPrice="
                             << data.LastPrice << " Volume=" << data.Volume;
                    result += data;
                }

                df.close();

            }
        }

    }

    return result;

}

//Q_EXPORT_PLUGIN2(DataAccessLayer, DataAccessLayerPlugin)
