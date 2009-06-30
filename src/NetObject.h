#ifndef NETOBJECT_H
#define NETOBJECT_H

#include <enet/enet.h>

namespace Ap {

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
    // What does this create do??
    virtual NetObject *create(int id) = 0;

    virtual int advance(float dt) { return 0; };    // Predictably update the object // Not necessary to overwrite
                                                    // 0: OK. -1: Object requests destruction.
};

}
#endif
