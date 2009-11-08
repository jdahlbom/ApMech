#ifndef AP_FUNCTIONS_H
#define AP_FUNCTIONS_H

#include <string>
#include <sstream>
#include <iostream>

#include "types.h"
#include <enet/enet.h> // TODO: Why do we need this one?

#ifndef WIN32
#include <Ogre.h>
#else
#include <Ogre/Ogre.h>
#endif

namespace ap {

std::string uint2ipv4(unsigned int ip);  // Convert a uint32 to a string with the corresponding IP address

void hexprint(enet_uint8 *buffer, int length); // Cout a random buffer and maybe understand what's in it!

/**
 * generic to_string() for any pipeable type.
 */
template <class T> inline std::string to_string (const T &t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <class T> bool from_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

void mSleep(ap::uint32 milliseconds);
void uSleep(ap::uint32 microseconds);
ap::uint32 getTicks();

ap::uint32 getColorFromPseudoHue(float phue);

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE

// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the current working directory
// for locating your configuration files and resources.
std::string macBundlePath();
#endif

} // namespace ap

#endif
