#ifndef __location_h__
#define __location_h__

#include <cmath>     // for sin and cos, for advance
#include <iostream>
#include <enet/enet.h>

class Location {
 public:

    float x, y, z, xvel, yvel, zvel, heading;
    float a, turning; // These are needed here ONLY for a smooth advance function.

    Location();

    void setLocation(float _x, float _y, float _z) { x = _x; y = _y; z = _z; }
    void setVelocity(float _xv, float _yv, float _zv) { xvel = _xv; yvel = _yv; zvel = _zv; }

    int serialize(enet_uint8 buffer[], int start, int buflength);
    int unserialize(enet_uint8 buffer[], int start);

    void advance(float dt);

    Location operator=(Location proto);
    friend std::ostream &operator<<(std::ostream &stream, Location l);
};

std::ostream &operator<<(std::ostream &stream, Location l);

#endif
