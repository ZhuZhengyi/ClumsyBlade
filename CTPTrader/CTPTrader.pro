
TEMPLATE = app
TARGET   = ClumsyBlade
QT       += core gui xml sql
DESTDIR  = ../bin
DEFINES  += TEST

#CONFIG  += localize_deployment

win32 {
LIBS += $${PWD}/lib/lua52.lib \
    $${PWD}/lib/thosttraderapi.lib \
    $${PWD}/lib/thostmduserapi.lib
}

unix {
LIBS += \
    $${PWD}/lib/thosttraderapi.so \
    $${PWD}/lib/thostmduserapi.so
}


SOURCES += \
    ../ShareLib/share_headers/utils/lessthan.cpp \
    ../ShareLib/share_headers/utils/filesystemhelper.cpp \
    ../ShareLib/share_headers/utils/datamanager.cpp \
    ../ShareLib/share_headers/utils/StringUtil.cpp \
    addressinfo.cpp \
    ibtrader.cpp \
    main.cpp \
    ctpwrapper.cpp \
    ctpmdwrapper.cpp \
    ctptraderwrapper.cpp \
    parser/brokerparser.cpp \
    UI/klinechart.cpp \
    UI/connectdlg.cpp \
    UI/aboutdlg.cpp \
    UI/tickchart.cpp \
    UI/stocklist.cpp \
    UI/frame.cpp \
    UI/strategyeditor.cpp \
    UI/pvsplashscreen.cpp \
    utils/highlighter.cpp

HEADERS     += \
    ../ShareLib/share_headers/utils/lessthan.h \
    ../ShareLib/share_headers/utils/filesystemhelper.h \
    ../ShareLib/share_headers/utils/datamanager.h \
    ../ShareLib/share_headers/utils/StringUtil.h \
    ibtrader.h \
    ctpwrapper.h \
    ctpmdwrapper.h \
    ctptraderwrapper.h \
    parser/brokerparser.h \
    PVSConstant.h \
    addressinfo.h \
    CommonStrut.h \
    UI/klinechart.h \
    UI/connectdlg.h \
    UI/aboutdlg.h \
    UI/tickchart.h \
    UI/stocklist.h \
    UI/frame.h \
    UI/strategyeditor.h \
    UI/pvsplashscreen.h \
    utils/highlighter.h

FORMS       += \
    UI/ibtrader.ui \
    UI/tickchart.ui \
    UI/connectdlg.ui \
    UI/aboutdlg.ui \
    UI/frame.ui \
    UI/strategyeditor.ui

RESOURCES   += \
    ibtrader.qrc

RC_FILE     = myapp.rc

TRANSLATIONS += \
    lang/en_US.ts \
    lang/zh_CN.ts

OTHER_FILES += \
    lang/zh_CN.ts \
    lang/en_US.ts \
    conf/csharp.txt \
    conf/lua.txt \
    conf/GmodLua.xml \
    conf/pvsniper.ini \
    conf/brokers.xml
