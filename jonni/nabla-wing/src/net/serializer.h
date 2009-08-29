#ifndef AP_SERIALIZER
#define AP_SERIALIZER

//NABLA #include <Ogre.h>

#include "../types.h"

namespace ap {
namespace net {

//typedef unsigned char uint8;

//NABLA int serialize(const Ogre::Vector3 &vect, uint8 *buffer, int start, int buflength);
//NABLA int serialize(const Ogre::Quaternion &quat, uint8 *buffer, int start, int buflength);
int serialize(float f, uint8 *buffer, int start, int buflength);
int serialize(int32 i, uint8 *buffer, int start, int buflength);
int serialize(uint8 i, uint8 *buffer, int start, int buflength);
int serialize(uint32 i, uint8 *buffer, int start, int buflength);

//NABLA int unserialize(Ogre::Vector3 &vect, uint8 *buffer, int start);
//NABLA int unserialize(Ogre::Quaternion &quat, uint8 *buffer, int start);
int unserialize(float &f, uint8 *buffer, int start);
int unserialize(int32 &i, uint8 *buffer, int start);
int unserialize(uint8 &i, uint8 *buffer, int start);
int unserialize(uint32 &i, uint8 *buffer, int start);
} // namespace net
} // namespace ap

#endif
