#ifndef __netgameobject_h__
#define __netgameobject_h__

#include <list>
#include "net/netdata.h"
#include "types.h"
#include "constants.h"
#include "location.h"
#include "projectile.h"
#include "NablaControl.h"
#include <iostream>

using namespace ap;
using namespace ap::net;

class NetGameObject : public NetObject {

 public:

    int color;      // transported over network
    float mg_delay;
    Location loc;
    Controller *controls;

    NetGameObject(int _id, int _uid = -1);

    uint8 getObjectType();

    int serialize(ap::uint8 buffer[], int start, int buflength) const;
    int unserialize(ap::uint8 buffer[], int start);
    NetObject *create(ap::uint32 id);

    int advance(float dt);
    list<NetObject *> *control(NetUser &user, bool createobjects = false);
};

#endif
