#ifndef __netuser_h__
#define __netuser_h__

#include "netobject.h"
#include <enet/enet.h>
#include <string>
#include <iostream>

using namespace std;

class NetUser : public NetObject
{
 public:

    ENetPeer peer;
    int uid;
    std::string nick;

    int x, y, color, ping;
    float a, turning;       // m/s², and rad/s

    NetUser();
    NetUser(int uid, ENetPeer _peer);

    int serialize(enet_uint8 buffer[], int start, int buflength);
    int unserialize(enet_uint8 buffer[], int start);
    NetObject *create(int id);
};


#endif
