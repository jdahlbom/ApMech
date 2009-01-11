#include "netuser.h"

NetUser::NetUser()
{
    uid = -1; x = y = 0; nick = "uninitialized";
    color = 0x00FFFFFF; ping = 0;
    changed = false;
}

NetUser::NetUser(int _uid, ENetPeer _peer)
{
    x = y = 0;  nick = "uninitialized";
    color = 0x00FFFFFF; ping = 0;
    uid = _uid;
    peer = _peer;
    changed = false;
}

int NetUser::serialize(enet_uint8 buffer[], int start, int buflength)
{
    int length = 0;

    *(int *)(buffer+start+length) = uid;                    length += 4;
    *(int *)(buffer+start+length) = x;                      length += 4;
    *(int *)(buffer+start+length) = y;                      length += 4;
    *(int *)(buffer+start+length) = color;                  length += 4;
    *(int *)(buffer+start+length) = ping;                   length += 4;
    *(float *)(buffer+start+length) = a;                    length += 4;
    *(float *)(buffer+start+length) = turning;              length += 4;
    strcpy( (char *)buffer + start+length, nick.c_str());   length += nick.length()+1;

    return length;
}


int NetUser::unserialize(enet_uint8 buffer[], int start)
{
    int length = 0;

    if (uid == *(int *)(buffer+start)) {                    length += 4;
        x = *(int *)(buffer+start+length);                  length += 4;
        y = *(int *)(buffer+start+length);                  length += 4;
        color = *(int *)(buffer+start+length);              length += 4;
        ping = *(int *)(buffer+start+length);               length += 4;
        a = *(float *)(buffer+start+length);                length += 4;
        turning = *(float *)(buffer+start+length);          length += 4;
        nick.assign((char *)buffer+start+length);           length += nick.length()+1;

//        cout << uid << ": x "<<x<<", y "<<y<<" nick "<<nick<<endl;
    } else cout << "FOULED!" << endl;

    return length;
}


NetObject *NetUser::create(int id) // OBS! FIXME! A rather dummy method. NetUser without uid is baddy.
{
    NetObject *ptr = new NetUser();
    return ptr;
}
