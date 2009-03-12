#ifndef __starfield_h__
#define __starfield_h__

#include <cmath>     // for sin and cos, for advance
#include "netdata.h"
#include "netobject.h"
#include "location.h"

#include <SDL.h>

class StarField : public NetObject {
 public:
    StarField(int _id, int _uid = -1);

    int serialize(enet_uint8 buffer[], int start, int buflength);
    int unserialize(enet_uint8 buffer[], int start);
    NetObject *create(int id);

    int draw(SDL_Surface *s, float x, float y);
};

#endif
