#ifndef AP_CONTROLLER_H
#define AP_CONTROLLER_H

#include "Serializable.h"
#include "../types.h"

namespace ap {
namespace net {

/**
 * Controller abstraction exists to help compiler detect invalid assignments to NetObject controls.
 **/

class Controller : public ap::net::Serializable
{
    public:
    Controller() {}
    virtual ~Controller() {}

    virtual int serialize(uint8 buffer[], int start, int buflength) const = 0;
    virtual int unserialize(uint8 buffer[], int start) = 0;
};

} // namespace net
} // namespace ap

#endif
