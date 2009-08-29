#ifndef AP_NET_NETEVENT_H
#define AP_NET_NETEVENT_H

#include "../types.h"

namespace ap {
namespace net {

class NetEvent
{
 public:
    ap::uint8 type;
    ap::uint32 uid;
    void *data;

    NetEvent();
    NetEvent(ap::uint8 _type, ap::int64);

};

} // namespace net
} // namespace ap

#endif
