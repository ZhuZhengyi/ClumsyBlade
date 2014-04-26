#ifndef CONNECTDLG_H
#define CONNECTDLG_H

#include <QtGui>
#include <QtXml>
#include "addressinfo.h"

namespace Ui {
class ConnectDlg;
}

class ConnectDlg : public QDialog
{
    Q_OBJECT

private:
    Ui::ConnectDlg *ui;
    void initComboBox();
    QList<AddressInfo> marketDataAddrList;
    QList<AddressInfo> tradingAddrList;
    QString BrokerID;
    QString BrokerName;
    
public:
    explicit ConnectDlg(QWidget *parent = 0);
    ~ConnectDlg();

    QString getMdAddr();
    QString getTradingAddr();
    QString getBrokerId();
    QString getUserName();
    QString getPwd();

private slots:

    void onCancel();
    void currentIndexChangedSlot(int);
};

#endif // CONNECTDLG_H
