#ifndef __projectile_h__
#define __projectile_h__

#include <cmath>     // for sin and cos, for advance
#include "net/netdata.h"
#include "location.h"
#include "constants.h"

#include <SDL.h>

using namespace ap;
using namespace ap::net;

class Projectile : public NetObject {
 public:

    Location loc;
    float age;   // age of the object, in seconds.

    Projectile(int _id, int _uid = -1);

    uint8 getObjectType();

    int serialize(ap::uint8 buffer[], int start, int buflength) const;
    int unserialize(ap::uint8 buffer[], int start);
    NetObject *create(ap::uint32 id);

    int advance(float dt);
};

#endif
