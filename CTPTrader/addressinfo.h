#ifndef ADDRESS_INFO_H
#define ADDRESS_INFO_H

#include <QtCore>

enum AddrType
{
	Trading = 0,
	MarketData
};

class AddressInfo
{
public:
	AddressInfo();

private:
	QString name;
	QString ipAddr;
	AddrType addType;

public:
	void setName(QString name);

	QString getName();

	void setIpAddr(QString ipAddr);

	QString getIpAddr();

	void setAddrType(AddrType type);

	AddrType getAddType();
};

#endif