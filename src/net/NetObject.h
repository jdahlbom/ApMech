#ifndef NETOBJECT_H
#define NETOBJECT_H

#include "Serializable.h"
#include "../types.h"

namespace ap {
namespace net {

class NetObject : public Serializable {
 public:
    ap::uint32 id;
    ap::int32 uid;            // owner
    bool changed;       // Whether the object was updated since last serialization

    NetObject(int _id = -1) { id = _id; }
    inline bool isChanged() { return changed; }
    inline void setChanged() { changed = true; }
    virtual uint8 getObjectType() = 0;

    /// any class that inherits GameObject must be ready to serialize!
    virtual int serialize(ap::uint8 buffer[], int start, int buflength) const = 0;
    virtual int unserialize(ap::uint8 buffer[], int start) = 0;

    /** A method for an object to create an object of its own type, and return
     * it as a NetObject*. Needed for unserialization.
     */
    virtual NetObject *create(ap::uint32 id) = 0;

    /** Predictably update the object.
     * Not necessary to overwrite if your object's state is not a (predictable)
     * function of time.
     * \return int. 0: OK. -1: Object requests its own destruction.
     */
    virtual int advance(float dt) { return 0; };
};

} // namespace net
} // namespace ap

#endif
