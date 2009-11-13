#include "NetUser.h"

#include "serializer.h"
#include "Controller.h"
#include "../constants.h"

namespace ap {
namespace net {

NetUser::NetUser():
    peer(0),
    uid(0),
    ping(0),
    initialized(false),
    color(0xFFFFFF),
    nick("uninitialized"),
    chosenVehicleType("")
{
    controls = 0;
    changed = false;
}

NetUser::NetUser(uint32 _uid, ENetPeer *_peer) :
    peer(_peer),
    uid(_uid),
    ping(0),
    initialized(false),
    color(0xFFFFFF),
    nick("uninitialized"),
    chosenVehicleType("")
{
    controls = 0;
    changed = false;
}

ap::uint8 NetUser::getObjectType() const {
  return ap::OBJECT_TYPE_NETUSER;
}

void NetUser::setControlPtr(Controller *ctrl)
{
    controls = ctrl;
}

int NetUser::serialize(enet_uint8 buffer[], int start, int buflength) const
{
    using ap::net::serialize;
    int length = 0;

    length += serialize(uid, buffer, start+length, buflength-length);
    length += serialize(ping, buffer, start+length, buflength-length);
    length += serialize(color, buffer, start+length, buflength-length);

    if(controls) {
        *(buffer + start + length) = CONTROL_IS_SET; ++length;
        length += controls->serialize(buffer, start+length, buflength-length);
    } else {
        *(buffer + start + length) = CONTROL_NOT_SET; ++length;
    }
    *(buffer+start+length) = CONTROL_BLOCK_FINISHED; ++length;
    length += serialize(nick, buffer, start+length, buflength);
    length += serialize(chosenVehicleType, buffer, start+length, buflength);

    return length;
}


int NetUser::unserialize(enet_uint8 buffer[], int start)
{
    using ap::net::unserialize;
    int length = 0;
    length += unserialize(uid, buffer, start+length);
    initialized = true;

    if (uid) {
        length += unserialize(ping, buffer, start+length);
        length += unserialize(color, buffer, start+length);

        // buffer must have control set AND controls pointer must not point to null.
        if (CONTROL_IS_SET == *(buffer+start+(length++))) {
            if (controls) {
                length += controls->unserialize(buffer, start+length);
                ++length; // For the CONTROL_BLOCK_FINISHED byte.
            } else {
                std::cout << "[NETUSER] unserialize - this block shouldn't get executed." << std::endl;
                // TODO: What if the CONTROL_BLOCK_FINISHED goes missing in transit? (kaboom?)
                // TODO: And hmm, yeah. the CONTROL_BLOCK_FINISHED just might not be very unique as a byte...
                while( 1==1 ) {
                    if (CONTROL_BLOCK_FINISHED == buffer[start+length]) {
                        ++length;
                        break;
                    }
                    ++length;
                }
            }
        }

        length += unserialize(nick, buffer, start+length);
	length += unserialize(chosenVehicleType, buffer, start+length);
//        cout << uid << ": x "<<x<<", y "<<y<<" nick "<<nick<<endl;
    } else cout << "FOULED in NetUser::unserialize!" << endl;

    return length;
}


NetObject *NetUser::create(uint32 id) const // OBS! FIXME! A rather dummy method. NetUser without uid is baddy.
{
    NetObject *ptr = new NetUser();
    return ptr;
}

} // namespace net
} // namespace ap
