#include "NetUser.h"

#include "serializer.hpp"

namespace ap {   
namespace net {

NetUser::NetUser()
{
    uid = -1; nick = "uninitialized";
    ping = 0;
    changed = false;
}

NetUser::NetUser(int _uid, ENetPeer *_peer)
{
    nick = "uninitialized";
    ping = 0;
    uid = _uid;    peer = _peer;
    controls = 0;
    changed = false;
}

int NetUser::serialize(enet_uint8 buffer[], int start, int buflength) const
{
    int length = 0;

    length += serialize(uid, buffer, start+length, buflength-length);
    length += serialize(ping, buffer, start+length, buflength-length);
    length += serialize(a, buffer, start+length, buflength-length);
    length += serialize(turning, buffer, start+length, buflength-length);
  //*(unsigned int *)(buffer+start+length) = controls;      length += 4;
    strcpy( (char *)buffer + start+length, nick.c_str());   length += nick.length()+1;

    return length;
}


int NetUser::unserialize(enet_uint8 buffer[], int start)
{
    int length = 0;
    length += unserialize(uid, buffer, start+length, buflength-length);

    if (uid) {
        length += unserialize(ping, buffer, start+length);
        length += unserialize(a, buffer, start+length);
        length += unserialize(turning, buffer, start+length);
  //    controls = *(unsigned int *)(buffer+start+length);  length += 4;
        nick.assign((char *)buffer+start+length);           length += nick.length()+1;

//        cout << uid << ": x "<<x<<", y "<<y<<" nick "<<nick<<endl;
    } else cout << "FOULED in NetUser::unserialize!" << endl;

    return length;
}


NetObject *NetUser::create(int id) // OBS! FIXME! A rather dummy method. NetUser without uid is baddy.
{
    NetObject *ptr = new NetUser();
    return ptr;
}

} // namespace net
} // namespace ap
