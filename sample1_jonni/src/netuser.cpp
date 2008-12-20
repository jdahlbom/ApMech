#include "netuser.h"

NetUser::NetUser()
{
    uid = x = y = -1; nick = "uninitialized";
    changed = false;
}

NetUser::NetUser(int _uid, ENetPeer _peer)
{
    x = y = -1;  nick = "uninitialized";
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
    strcpy( (char *)buffer + start+length, nick.c_str());   length += nick.length()+1;

    return length;
}


int NetUser::serialset(enet_uint8 buffer[], int start)
{
    int length = 0;

    if (uid == *(int *)(buffer+start)) {                    length += 4;
        x = *(int *)(buffer+start+length);                  length += 4;
        y = *(int *)(buffer+start+length);                  length += 4;
        color = *(int *)(buffer+start+length);              length += 4;
        nick.assign((char *)buffer+start+length);           length += nick.length()+1;

//        cout << uid << ": x "<<x<<", y "<<y<<" nick "<<nick<<endl;
    } else cout << "FOULED!" << endl;

    return length;
}
