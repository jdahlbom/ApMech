#include "NetEvent.h"
#include "netdata.h"

namespace ap {
namespace net {

NetEvent::NetEvent() {
    type = NetData::EVENT_NOEVENT;
}

NetEvent::NetEvent(enet_uint8 _type, int a) {
    type = _type;

    switch (type)
    {
     case NetData::EVENT_CONNECT:
        uid = a;
        break;
     default:
        break;
    }
}

} // namespace net
} // namespace net
