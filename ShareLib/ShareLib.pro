QT += core xml
TEMPLATE = app
CONFIG += console

#DESTDIR = ../bin
DESTDIR = ../release

win32 {

LIBS += \
    F:/myfile/ming/lib/thosttraderapi.lib \
    F:/myfile/ming/lib/thostmduserapi.lib

}

unix {

LIBS += \
        /usr/local/lib/thosttraderapi.so \
        /usr/local/lib/thostmduserapi.so

}

SOURCES += \
    share_headers/utils/lessthan.cpp \
    share_headers/utils/filesystemhelper.cpp \
    main.cpp \
    share_headers/utils/StringUtil.cpp \
    share_headers/data_header/model/addressinfo.cpp \
    share_headers/utils/datamanager.cpp \
    share_headers/etiger_ctp/CTP_DataStruct.cpp \
    share_headers/utils/threadutils.cpp

HEADERS += \
    share_headers/ctp_headers/ThostFtdcUserApiStruct.h \
    share_headers/ctp_headers/ThostFtdcUserApiDataType.h \
    share_headers/ctp_headers/ThostFtdcTraderApi.h \
    share_headers/ctp_headers/ThostFtdcMdApi.h \
    share_headers/data_header/CommonStrut.h \
    share_headers/lua_include/lualib.h \
    share_headers/lua_include/luaconf.h \
    share_headers/lua_include/lua.hpp \
    share_headers/lua_include/lua.h \
    share_headers/lua_include/lauxlib.h \
    share_headers/utils/lessthan.h \
    share_headers/utils/filesystemhelper.h \
    share_headers/utils/EtigerKey.h \
    share_headers/utils/StringUtil.h \
    share_headers/data_header/model/basemodel.h \
    share_headers/data_header/model/addressinfo.h \
    share_headers/utils/datamanager.h \
    share_headers/etiger_ctp/CTP_DataStruct.h \
    share_headers/utils/threadutils.h

