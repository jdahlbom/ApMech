#ifndef NETOBJECT_H
#define NETOBJECT_H

#include "Serializable.h"
#include "../types.h"

namespace ap {
namespace net {

class NetObject : public Serializable {
 public:
    ap::uint32 id;
    ap::uint32 uid;            // owner
    bool changed;       // Whether the object was updated since last serialization

    NetObject(uint32 _id = 0) { id = _id; }
    inline bool isChanged() { return changed; }
    inline void setChanged() { changed = true; }
    virtual uint8 getObjectType() const = 0;

    /** A method for an object to create an object of its own type, and return
     * it as a NetObject*. Needed for unserialization.
     */
    virtual NetObject *create(ap::uint32 id) const = 0;

    /** Predictably update the object.
     * Not necessary to overwrite if your object's state is not a (predictable)
     * function of time.
     * \return int. 0: OK. -1: Object requests its own destruction.
     */
    virtual int advance(float dt) { return 0; };

    // interface inherited from Serializable
    virtual int serialize(uint8 buffer[], int start, int buflength) const = 0;
    virtual int unserialize(uint8 buffer[], int start) = 0;
};

} // namespace net
} // namespace ap

#endif
