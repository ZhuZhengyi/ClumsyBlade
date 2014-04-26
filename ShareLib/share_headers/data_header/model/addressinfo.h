#ifndef ADDRESSINFO_H
#define ADDRESSINFO_H

#include "basemodel.h"

enum AddrType
{
    Trading = 0,
    MarketData
};

class AddressInfo : public BaseModel
{
public:
    AddressInfo();

private:
	std::string name;
    std::string ipAddr;
    AddrType addType;

public:
    void setName(std::string name);
    std::string getName();

    void setIpAddr(std::string ipAddr);
    std::string getIpAddr();

    void setAddrType(AddrType type);
    AddrType getAddType();
};

#endif // ADDRESSINFO_H
