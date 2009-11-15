#ifndef AP_DEFINITIONS_H
#define AP_DEFINITIONS_H

/*
 * The purpose of this file is to manage all
 * platform-dependent definitions, so that
 * single source files need not use OGRE_PLATFORM,
 * or similar.
 */

#ifndef WIN32
#include <OgrePlatform.h>
#else
#include <Ogre/OgrePlatform.h>
#endif

#define AP_PLATFORM_WIN32 1
#define AP_PLATFORM_OSX   2
#define AP_PLATFORM_POSIX 3

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
  #define AP_PLATFORM AP_PLATFORM_OSX
#elif OGRE_PLATFORM == OGRE_PLATFORM_WIN32
  #define AP_PLATFORM AP_PLATFORM_WIN32
#else
  #define AP_PLATFORM AP_PLATFORM_LINUX
#endif


#endif
