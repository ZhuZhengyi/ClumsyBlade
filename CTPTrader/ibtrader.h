#ifndef IBTRADER_H
#define IBTRADER_H

#include <QtGui>

#include "PVSConstant.h"

#include "ui_ibtrader.h"
#include "../ShareLib/share_headers/ctp_headers/ThostFtdcUserApiStruct.h"

class CTPMdWrapper;
class CTPTraderWrapper;


class IBTrader : public QMainWindow
{
	Q_OBJECT

public:
	IBTrader(QWidget *parent = 0, Qt::WFlags flags = 0);
	~IBTrader();

    void releaseRes();
    void initRes();

private:

    CTPMdWrapper *mdWrapper;
    CTPTraderWrapper *traderWrapper;

    Ui::PVTraderClass ui;

    QString checkValue(double val) const
    {
        if(val >= 1.79769e+308)
        {
            return "---";
        }
        return QString::number(val);
    }

    QString getStyleSheetByVal(const double val, const double base) const
    {
        if((val >= 1.79769e+308) || (val == base))
        {
            return "QLabel{color:white}";
        }
        else if(val < base)
        {
            return "QLabel{color:green}";
        }

        return "QLabel{color:red}";
    }

public slots:
    void setNewState(PVSniper::PVS_STATE state);

    void actionConnect();
    void actionOrderInsert();
    void actionDisconnect();
    void actionExit();
    void actionAbout();
    void actionQryExchange();
    void actionQryInstrument();
    void actionQryInstrumentCommissionRate();
    void actionQryInstrumentMarginRate();
    void actionBankToFuture();
    void actionDemo();
    void actionLeftSlot(bool);
    void actionLeftSlot2(bool);
    void actionRightSlot(bool);
    void actionRightSlot2(bool);
    void actionBottomSlot(bool);
    void actionBottomSlot2(bool);

    void output2Log(QString text, bool append=true);
    void addInstrument(QString item, QString ExchangeID);
    void updateData(CThostFtdcDepthMarketDataField *);
    void clearLog();
    void updatePanel(CThostFtdcDepthMarketDataField*);
    void switchToTickChart(QString);
    void onLoginMDSlot(bool);
    void onLoginTraderSlot(bool);
    void onTraderDisconnectedSlot();
    void onTraderUserLogoutSlot();

signals:
    void updateTickSignal(CThostFtdcDepthMarketDataField*);

};

#endif // IBTRADER_H
