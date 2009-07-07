#include "MovingObject.hpp"


#include <Ogre.h>

namespace ap {

class Mech : public MovingObject
{
 public:
    Mech(Ogre::Vector3 velocity);
    ~Mech() {}

    net::NetObject *create(int id);
};

} // namespace ap
