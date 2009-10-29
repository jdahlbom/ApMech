#ifndef AP_SERIALIZER
#define AP_SERIALIZER

// If this library is used for other purposes than Apmech, define NO_OGRE
#ifndef NO_OGRE
 #ifndef WIN32
  #include <Ogre.h>
 #else
  #include <Ogre/Ogre.h>
 #endif
#endif

#include <string>
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
int serialize(std::string s, uint8 *buffer, int start, int buflength);

int unserialize(float &f, uint8 *buffer, int start);
int unserialize(int16 &i, uint8 *buffer, int start);
int unserialize(int32 &i, uint8 *buffer, int start);
int unserialize(uint8 &i, uint8 *buffer, int start);
int unserialize(uint16 &i, uint8 *buffer, int start);
int unserialize(uint32 &i, uint8 *buffer, int start);
int unserialize(std::string &s, uint8 *buffer, int start);

} // namespace net
} // namespace ap

#endif

