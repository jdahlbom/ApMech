#include "NetUser.h"

#include "serializer.hpp"

#include "../Controller.h"

namespace ap {
namespace net {

NetUser::NetUser():
    peer(0),
    uid(-1),
    nick("uninitialized"),
    ping(0)
{
    controls = 0;
    changed = false;
}

NetUser::NetUser(int _uid, ENetPeer *_peer) :
    peer(_peer),
    uid(_uid),
    nick("uninitialized"),
    ping(0)
{
    controls = 0;
    changed = false;
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

    if(controls) {
        std::cout << "[NETUSER] Controls ptr points to " << controls << std::endl;
        *(buffer + start + length) = CONTROL_IS_SET; ++length;
        length += controls->serialize(buffer, start+length, buflength-length);
    } else {
        *(buffer + start + length) = CONTROL_NOT_SET; ++length;
    }
    *(buffer+start+length) = CONTROL_BLOCK_FINISHED; ++length;
    strcpy( (char *)buffer + start+length, nick.c_str());   length += nick.length()+1;

    return length;
}


int NetUser::unserialize(enet_uint8 buffer[], int start)
{
    using ap::net::unserialize;
    int length = 0;
    length += unserialize(uid, buffer, start+length);

    if (uid) {
        length += unserialize(ping, buffer, start+length);

        // buffer must have control set AND controls pointer must not point to null.
        if (CONTROL_IS_SET == *(buffer+start+(length++))) {
            if (0 != controls) {
                std::cout << "[NETUSER] Controls points to " << controls << " and gets unserialized. " << std::endl;
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

        nick.assign((char *)buffer+start+length);           length += nick.length()+1;

//        cout << uid << ": x "<<x<<", y "<<y<<" nick "<<nick<<endl;
    } else cout << "FOULED in NetUser::unserialize!" << endl;

    return length;
}


NetObject *NetUser::create(uint32 id) // OBS! FIXME! A rather dummy method. NetUser without uid is baddy.
{
    NetObject *ptr = new NetUser();
    return ptr;
}

} // namespace net
} // namespace ap
