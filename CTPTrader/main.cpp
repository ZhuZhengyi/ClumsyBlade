#include "ibtrader.h"
#include <QtGui>
#include <QTranslator>
#include "UI/pvsplashscreen.h"
#include "../ShareLib/share_headers/utils/datamanager.h"

int main(int argc, char *argv[])
{
    //qDebug() << "start";

    QApplication app(argc, argv);
    //QApplication::addLibraryPath("./sqldrivers");

    QTextCodec *textc = QTextCodec::codecForName("gbk");
    QTextCodec::setCodecForCStrings(textc);

    QTranslator translator;
    translator.load("zh_CN");
    app.installTranslator(&translator);

    QFont font("Times", 10, QFont::Normal, FALSE);
    app.setFont(font);

    QPixmap pixmap("images/splash.png");
    PVSplashScreen *splash = new PVSplashScreen(pixmap);

    int MaxT = 1500;

    splash->setRange(0, MaxT);
    splash->show();

    QTime time;

    time.start();
    int tStep = MaxT / 8;
    bool unInit = true;
    int s = 1;
    int t = 3;

    while (time.elapsed() < tStep * (s + t))
    {
        splash->setProgress(time.elapsed());
        if(time.elapsed() > tStep * s && unInit)
        {
            Qt::Alignment topRight = Qt::AlignTop | Qt::AlignHCenter;
            splash->showMessage(QObject::tr("Loading Script Engine Module..."),topRight, Qt::white);
            DataManager::GetInstance()->loadScriptEngine(PVSniper::MOD_LUA);
            qDebug() << "load script engine done!";
            DataManager::GetInstance()->loadMatchList(PVSniper::MATCH_LIST);
            qDebug() << "load match list done!";
            unInit = false;
        }
        QCoreApplication::processEvents();
    }

    splash->clearMessage();
    unInit = true;
    s += t + 1;

    while (time.elapsed() < tStep * (s + t))
    {
        splash->setProgress(time.elapsed());
        if(time.elapsed() > tStep * s && unInit)
        {
            Qt::Alignment topRight = Qt::AlignTop | Qt::AlignHCenter;
            splash->showMessage(QObject::tr("Loading Data Access Module..."),topRight, Qt::white);
            DataManager::GetInstance()->loadDAL(PVSniper::APP_SETTING);
            qDebug() << "load Data Access Module done!";
            DataManager::GetInstance()->loadFrontAddresses(PVSniper::BROKERS_PATH);
            qDebug() << "load Front Addresses done!";
            unInit = false;
        }
        QCoreApplication::processEvents();
    }

    splash->clearMessage();

    while (time.elapsed() < MaxT)
    {
        splash->setProgress(time.elapsed());
    }

    IBTrader window;
    window.show();
    splash->finish(&window);

    delete splash;

    return app.exec();
}

