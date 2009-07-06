#ifndef AP_SERIALIZER
#define AP_SERIALIZER

#include <Ogre.h>

namespace ap {
namespace net {

typedef unsigned char uint8;

int serialize(const Ogre::Vector3 &vect, uint8 *buffer, int start, int buflength);
int serialize(const Ogre::Quaternion &quat, uint8 *buffer, int start, int buflength);
int serialize(float f, uint8 *buffer, int start, int buflength);

int unserialize(Ogre::Vector3 &vect, uint8 *buffer, int start);
int unserialize(Ogre::Quaternion &quat, uint8 *buffer, int start);
int unserialize(float &f, uint8 *buffer, int start);
} // namespace net
} // namespace ap

#endif
