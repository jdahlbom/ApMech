#ifndef __starfield_h__
#define __starfield_h__

#include <cmath>     // for sin and cos, for advance
#include "net/netdata.h"
#include "location.h"

#include <SDL.h>

using namespace ap::net;

class StarField : public NetObject {
 public:
    StarField(int _id = -1, int _uid = -1);

    uint8 getObjectType();

    int serialize(ap::uint8 buffer[], int start, int buflength) const;
    int unserialize(ap::uint8 buffer[], int start);
    NetObject *create(ap::uint32 id);
};

#endif
