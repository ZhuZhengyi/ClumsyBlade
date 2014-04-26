#ifndef PVSCONSTANT_H
#define PVSCONSTANT_H

#include <QString>

namespace PVSniper
{


    enum PVS_STATE
    {
        DISCONNECTED = 0,
        CONNECTED = 1,
        ITEM_SELECTED = 1 << 1,
        IN_PROCESS = 1 << 2
    };

    const QString AppName = "ClumsyBlade";
    const QString MD_CON = "md_cons/";
    const QString TRADER_CON = "trader_cons/";
    const QString CURRENT_INSTRUMENTS = "current_instruments.txt";
    const QString BROKERS_PATH = "brokers.xml";
    const QString APP_SETTING="pvsniper.ini";
    const QString MATCH_LIST="concerns.txt";
    const QString MOD_LUA="GmodLua.xml";

}

#endif // PVSCONSTANT_H
