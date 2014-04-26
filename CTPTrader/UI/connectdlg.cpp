#include "connectdlg.h"
#include "ui_connectdlg.h"
#include "../ShareLib/share_headers/utils/datamanager.h"s

//#define TEST
// #define MY_SELF
// #define DEMO

ConnectDlg::ConnectDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDlg)
{
    ui->setupUi(this);

    marketDataAddrList = DataManager::GetInstance()->getMarketDataAddrList();
    tradingAddrList = DataManager::GetInstance()->getTradingAddrList();

#ifdef TEST
    ui->brokerName->setText(tr("sim"));
    ui->brokerID->setText("2030");
   // ui->userName->setText("000000002855");
    //ui->password->setText("123456");
    //ui->userName->setText("354315");
    ui->userName->setText("352032");
    ui->password->setText("888888");
#endif

    initComboBox();

    connect(ui->okBtn, SIGNAL(clicked()), this, SLOT(accept()));
    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(onCancel()));
    connect(ui->tradingAddressCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChangedSlot(int)));
}

void ConnectDlg::currentIndexChangedSlot(int index)
{
    switch(index)
    {
#ifdef TEST
    case 0:
        ui->brokerID->setText("2030");
        ui->brokerName->setText(tr("sim"));
        ui->userName->setText("000000002855");
        ui->password->setText("123456");
        break;
#endif
    default:
        ui->brokerID->setText(BrokerID);
        ui->brokerName->setText(BrokerName);
#ifdef MY_SELF
        ui->userName->setText("8002041107");
        ui->password->setText("");
#else
        ui->userName->setText("");
        ui->password->setText("");
#endif
    }
}

void ConnectDlg::initComboBox()
{

    AddressInfo info;

    for(int i=0; i<marketDataAddrList.count(); i++)
    {
        info=marketDataAddrList.at(i);
        ui->mdAddressCombo->addItem(QIcon(), info.getName());
    }

    for(int i=0; i<tradingAddrList.count(); i++)
    {
        info=tradingAddrList.at(i);
        ui->tradingAddressCombo->addItem(QIcon(), info.getName());
    }

}

QString ConnectDlg::getMdAddr()
{
    AddressInfo addr = marketDataAddrList.at(ui->mdAddressCombo->currentIndex());
    return addr.getIpAddr();
}

QString ConnectDlg::getTradingAddr()
{
    AddressInfo addr = tradingAddrList.at(ui->tradingAddressCombo->currentIndex());
    return addr.getIpAddr();
}

QString ConnectDlg::getBrokerId()
{
    return ui->brokerID->text();
}

QString ConnectDlg::getUserName()
{
    return ui->userName->text();
}

QString ConnectDlg::getPwd()
{
    return ui->password->text();
}
ConnectDlg::~ConnectDlg()
{
    delete ui;
}

void ConnectDlg::onCancel()
{
    reject();
}
