#ifndef NETOBJECT_H
#define NETOBJECT_H

#include <enet/enet.h>

#include <SDL.h>        // Damn! I was hoping this file would have no extra dependencies. There goes.
                        // Couldn't think of a better way to let objects say something about how they're
                        // drawn.

class NetObject {

 public:

    int id;
    int uid;            // owner
    bool changed;       // Whether the object was updated since last serialization

    NetObject(int _id = -1) { id = _id; }
    inline bool isChanged() { return changed; }
    inline void setChanged() { changed = true; }

    // any class that inherits GameObject must be ready to serialize!
    virtual int serialize(enet_uint8 buffer[], int start, int buflength) = 0;
    virtual int unserialize(enet_uint8 buffer[], int start) = 0;
    virtual NetObject *create(int id) = 0;

    virtual int advance(float dt) { return 0; };    // Predictably update the object // Not necessary to overwrite
                                                    // 0: OK. -1: Object requests destruction.
    virtual int draw(SDL_Surface *s, float x, float y) { return 0; } // Draw, if necessary! Return nonzero if something drawn
};

#endif
