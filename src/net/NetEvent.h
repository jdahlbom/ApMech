#ifndef AP_NET_NETEVENT_H
#define AP_NET_NETEVENT_H

#include "../types.h"

namespace ap {
namespace net {

class NetEvent
{
 public:
    ap::uint8 type;
    ap::int64 uid;

    NetEvent();
    NetEvent(ap::uint8 _type, ap::int64 a);

};

} // namespace net
} // namespace ap

#endif
