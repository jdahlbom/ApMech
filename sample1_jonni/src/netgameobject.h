#ifndef __netgameobject_h__
#define __netgameobject_h__

#include <vector>
#include <cmath>     // for sin and cos, for advance
#include "netdata.h"
#include "netobject.h"
#include "netuser.h"
#include "location.h"
#include "projectile.h"
#include <iostream>

class NetGameObject : public NetObject {

 public:

    int id, color;      // transported over network
    float mg_delay;

    Location loc;

    NetGameObject(int _id, int _uid = -1);

    int serialize(enet_uint8 buffer[], int start, int buflength);
    int unserialize(enet_uint8 buffer[], int start);
    NetObject *create(int id);

    int advance(float dt);
    vector<NetObject *> *control(NetUser &user);
};

#endif
