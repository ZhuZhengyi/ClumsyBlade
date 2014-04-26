#include "addressinfo.h"

AddressInfo::AddressInfo()
{
}

void AddressInfo::setName(QString name)
{
	this->name = name;
}

QString AddressInfo::getName()
{
	return name;
}

void AddressInfo::setIpAddr(QString ipAddr)
{
	this->ipAddr = ipAddr;
}

QString AddressInfo::getIpAddr()
{
	return ipAddr;
}

void AddressInfo::setAddrType(AddrType type)
{
	this->addType = type;
}

AddrType AddressInfo::getAddType()
{
	return addType;
}