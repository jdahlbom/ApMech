#include "NetEvent.h"
#include "netdata.h"

#include "../types.h"

namespace ap {
namespace net {

NetEvent::NetEvent() {
    type = NetData::EVENT_NOEVENT;
    message = NULL;
}

NetEvent::NetEvent(ap::uint8 _type, ap::int32 _uid) {
    type = _type;
    uid = _uid;
    message = NULL;
}

NetEvent::NetEvent(ap::uint8 _type, NetMessage *_message) {
    type = _type;
    message = new NetMessage(*_message);
}

NetEvent::NetEvent(NetEvent &e)
{
    type = e.type;  uid = e.uid;
    if (e.message) message = new NetMessage(*e.message);
}

void NetEvent::operator=(NetEvent &e)
{
    type = e.type;  uid = e.uid;
    if (message) { delete message; message = NULL; }
    if (e.message) message = new NetMessage(*e.message);
}

NetEvent::~NetEvent()
{
    if (message) delete message;
}

} // namespace net
} // namespace net
