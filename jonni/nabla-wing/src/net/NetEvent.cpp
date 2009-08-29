#include "NetEvent.h"
#include "netdata.h"

#include "../types.h"

namespace ap {
namespace net {

NetEvent::NetEvent() {
    type = NetData::EVENT_NOEVENT;
}

NetEvent::NetEvent(ap::uint8 _type, ap::int64 id) {
    type = _type;
    uid = id;
}

} // namespace net
} // namespace net
