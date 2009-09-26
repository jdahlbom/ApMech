#ifndef AP_SERIALIZER
#define AP_SERIALIZER

// Test if we're compiling with Ogre3D or not, and include accordingly.
// This is for the sake of ap::net code not depending on Ogre.
// I hope one of these is always defined, btw! Dunno for sure.
#if defined(OGRE_CONFIG_LITTLE_ENDIAN) || defined(OGRE_CONFIG_BIG_ENDIAN)
#include <Ogre.h>
#endif

#include "../types.h"

namespace ap {
namespace net {

#ifdef OGRE_VERSION
int serialize(const Ogre::Vector3 &vect, uint8 *buffer, int start, int buflength);
int serialize(const Ogre::Quaternion &quat, uint8 *buffer, int start, int buflength);
int unserialize(Ogre::Vector3 &vect, uint8 *buffer, int start);
int unserialize(Ogre::Quaternion &quat, uint8 *buffer, int start);
#endif

int serialize(float f, uint8 *buffer, int start, int buflength);
int serialize(int16 i, uint8 *buffer, int start, int buflength);
int serialize(int32 i, uint8 *buffer, int start, int buflength);
int serialize(uint8 i, uint8 *buffer, int start, int buflength);
int serialize(uint16 i, uint8 *buffer, int start, int buflength);
int serialize(uint32 i, uint8 *buffer, int start, int buflength);

int unserialize(float &f, uint8 *buffer, int start);
int unserialize(int16 &i, uint8 *buffer, int start);
int unserialize(int32 &i, uint8 *buffer, int start);
int unserialize(uint8 &i, uint8 *buffer, int start);
int unserialize(uint16 &i, uint8 *buffer, int start);
int unserialize(uint32 &i, uint8 *buffer, int start);
} // namespace net
} // namespace ap

#endif

