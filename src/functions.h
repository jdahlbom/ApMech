/*  Random small functions
 *
 *  Jonni Lehtiranta
 */

#include <string>
#include <sstream>
#include <iostream>
#include <enet/enet.h>
#include <sys/time.h>

#include <Ogre.h> // In order to use OGRE_PLATFORM we need Ogre.h

namespace ap {

std::string uint2ipv4(unsigned int ip);  // Convert a uint32 to a string with the corresponding IP address

void hexprint(enet_uint8 *buffer, int length); // Cout a random buffer and maybe understand what's in it!

template <class T> inline std::string to_string (const T &t); // generic to_string() for any pipeable thing

long int getTicks(); // milliseconds since approx program start. OBS! Not C-standard, but POSIX

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE

// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the current working directory
// for locating your configuration files and resources.
std::string macBundlePath();
#endif

} // namespace ap
