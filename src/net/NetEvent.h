#ifndef AP_NET_NETEVENT_H
#define AP_NET_NETEVENT_H

#include <enet/enet.h>

namespace ap {
namespace net {

class NetEvent
{
 public:
    enet_uint8 type;
    int uid;

    NetEvent();
    NetEvent(enet_uint8 _type, int a);

};

} // namespace net
} // namespace ap

#endif
