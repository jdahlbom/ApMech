#ifndef NETOBJECT_H
#define NETOBJECT_H

#include "Serializable.h"
#include "../types.h"
#include <map>

namespace ap {
namespace net {

class NetObject : public Serializable {
 public:
    ap::uint32 id;
    ap::uint32 pid;     // parent object id
    ap::uint32 uid;     // owner id
    bool changed;       // Whether the object was updated since last serialization

    NetObject(uint32 _id = 0, uint32 _pid = 0) { id = _id; pid = _pid;}
    inline bool isChanged() { return changed; }
    inline void setChanged() { changed = true; }
    /** Does this object belong under some other netobject in some hierarchy?
     *  If so, return its id. Otherwise return zero (default). */
    uint32 getParentId() { return pid; }

    /** A method for returning a unique type identifier as a 8-bit uint. */
    virtual uint8 getObjectType() const = 0;

    /** A method for an object to create an object of its own type, and return
     *  it as a NetObject*. Needed for unserialization.
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

// declared here, so that inheritants do not need to include netdata.h.
extern std::map<ap::uint8, NetObject *>& netobjectprototypes();


/* This mystical part here pushes exactly ONE object of this type to the prototype map,
 * in order to easily unserialize stuff. Idea gotten from C++ FAQ Lite by Marshall Cline.
 * It exploits the idea that static objects get constructed before the main program starts.
 * Take care to replace "MyNetObject" with the name of your actual object, amd place the
 * stuff below in the .cpp file for your object!
 */
/*
class MyNetObjectInject {
  public:
    MyNetObjectInject() {
      ap::net::netobjectprototypes().insert(
        std::make_pair(ap::OBJECT_TYPE_MyNetObject, new GameWorld()));
    }
};
static MyNetObjectInject __mynetobjectinject;
*/

} // namespace net
} // namespace ap

#endif
