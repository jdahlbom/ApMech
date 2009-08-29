#ifndef __NETMESSAGE_H__
#define __NETMESSAGE_H__

#include "Serializable.h"
#include "serializer.h"
#include "../types.h"

#include <list>
#include <string>

using namespace ap;
using namespace ap::net;

class NetMessage : public Serializable
{
    uint32 sender;
    std::list<uint32> *recipients;
    std::string data;

    NetMessage(std::string _data = "", uint32 s = 0, uint32 r = 0);
    NetMessage(std::string _data, uint32 s, std::list<uint32> &r);

    int serialize(ap::uint8 buffer[], int start, int buflength) const;
    int unserialize(ap::uint8 buffer[], int start);
};

#endif
