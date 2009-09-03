#ifndef AP_NET_NETEVENT_H
#define AP_NET_NETEVENT_H

#include "../types.h"
#include "NetMessage.h"
#include <iostream> // debug

namespace ap {
namespace net {

class NetEvent
{
 public:
    ap::uint8 type;
    ap::uint32 uid;
    NetMessage *message;

    NetEvent();
    NetEvent(ap::uint8 _type, ap::int32 _uid);
    NetEvent(ap::uint8 _type, NetMessage *_message);
    NetEvent(NetEvent &e);                        // Copy constructor
    void operator=(NetEvent &e);
    ~NetEvent();
};

} // namespace net
} // namespace ap

#endif
