#ifndef AP_FUNCTIONS_H
#define AP_FUNCTIONS_H

#include <string>
#include <sstream>
#include <iostream>

#include "types.h"
#include <enet/enet.h> // TODO: Why do we need this one?

namespace ap {

std::string uint2ipv4(unsigned int ip);  // Convert a uint32 to a string with the corresponding IP address

void hexprint(enet_uint8 *buffer, int length); // Cout a random buffer and maybe understand what's in it!
void bufprint(enet_uint8 *buffer, int length); // Same, but print bytes as decimal numbers

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

std::string bundlePath();

} // namespace ap

#endif
