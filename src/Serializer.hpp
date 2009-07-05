#ifndef AP_SERIALIZER
#define AP_SERIALIZER

#include <Ogre.h>
#include <enet/enet.h>

typedef unsigned char uint8;

namespace Ap {
namespace serializer {
int serialize(const Ogre::Vector3 &vect, uint8 *buffer, int start, int buflength);
int serialize(const Ogre::Quaternion &quat, uint8 *buffer, int start, int buflength);
int serialize(float f, uint8 *buffer, int start, int buflength);

int unserialize(Ogre::Vector3 &vect, uint8 *buffer, int start);
int unserialize(Ogre::Quaternion &quat, uint8 *buffer, int start);
int unserialize(float &f, uint8 *buffer, int start);
} // namespace serializer
} // namespace Ap

#endif
