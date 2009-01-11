#ifndef __netgameobject_h__
#define __netgameobject_h__

#include <map>
#include "netdata.h"
#include "netobject.h"

#include <iostream>

class NetGameObject : public NetObject {

 public:

    int id;
    int color;
    float x, y, z, xvel, yvel, zvel, heading;

    NetGameObject(int _id, int _uid = -1);

    void setLocation(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
    void setVelocity(float _xv, float _yv, float _zv) { xvel = _xv; yvel = _yv; zvel = _zv; }
    float getX() { return this->x; }
    float getY() { return this->y; }
    float getZ() { return this->z; }

    int serialize(enet_uint8 buffer[], int start, int buflength);
    int unserialize(enet_uint8 buffer[], int start);
    NetObject *create(int id);
};

#endif
