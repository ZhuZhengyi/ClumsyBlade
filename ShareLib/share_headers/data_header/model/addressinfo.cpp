#include "addressinfo.h"

AddressInfo::AddressInfo()
{
}


void AddressInfo::setName(std::string name)
{
    this->name = name;
}

std::string AddressInfo::getName()
{
    return name;
}

void AddressInfo::setIpAddr(std::string ipAddr)
{
    this->ipAddr = ipAddr;
}

std::string AddressInfo::getIpAddr()
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
