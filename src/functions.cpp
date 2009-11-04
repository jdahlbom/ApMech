#include "functions.h"


#include <string>
#include <sstream>
#include <iostream>

#ifndef WIN32
#include <Ogre.h>
#include <SDL.h>
#else
#include <Ogre/Ogre.h>
#include <SDL/SDL.h>
#endif

#include <enet/enet.h> // TODO: Why do we need this one?

namespace ap
{

using namespace std;

string uint2ipv4(unsigned int ip)
{
    stringstream out;
    out << (ip&255) <<"."<< ((ip>>8)&255) <<"."<< ((ip>>16)&255) <<"."<< (ip>>24);
    return out.str();
}


void hexprint(enet_uint8 *buffer, int length)
{
    enet_uint8 u;
    cout << "Buf length = "<<length<<": ";
    cout << "(";
    for (int i=0; i<length; i++) {
        u = buffer[i];

        if ((u > 64) && (u < 91)) cout << char(u);
        else if ((u > 96) && (u < 123)) cout << char(u);
        else cout << int(u);

        if (i != length-1) cout << ",";
    }

    cout << ")"<<endl;
}

/**
 * Sleep for defined amount of milliseconds
 * @param unsigned int milliseconds - The amount of milliseconds to sleep
 */
void mSleep(ap::uint32 milliseconds)
{
	SDL_Delay(milliseconds);
}

/**
 * Sleep for defined amount of microseconds
 * @param unsigned int microseconds - The amount of microseconds to sleep
 */
void uSleep(ap::uint32 microseconds)
{
	// TODO: use something more accurate, perhaps depending on OS
	SDL_Delay(microseconds / 1000);
}

/**
 * Return the time since program started in milliseconds.
 * @return ap::uint32 Time since program started in milliseconds
 */
ap::uint32 getTicks()
{
	return SDL_GetTicks();
/*
    static long int startticks = -1;
    static time_t starttime_t;
    static timeval now;
	
    gettimeofday(&now, NULL);

    if (startticks != -1) { // then the function is initialized already
        return (now.tv_sec - starttime_t)*1000 + (now.tv_usec - startticks)/1000;
    } else {
        starttime_t = now.tv_sec;
        startticks = now.tv_usec/1000;
        return 0;
    }
*/
}

/** Convert a pseudo-hue from a ColorSlider widget value (float, [0,1])
 * to uint32 RGBA. */
ap::uint32 getColorFromPseudoHue(float phue)
{
    if (phue < (6./96.)) return 0x00FFFFFF;     // White!

    float hue = 1. - (phue - (6./96.)) * (96./90.);
    uint8 r = 0, g = 0, b = 0;

    if (hue < 1./6.) {
        r = 0xFF;
        g = int(255. * 6. * hue);
    } else if (hue < 2./6.) {
        r = int(255. - 255. * 6. * (hue - 1./6.));
        g = 0xFF;
    } else if (hue < 3./6.) {
        g = 0xFF;
        b = int(255. * 6. * (hue - 2./6.));
    } else if (hue < 4./6.) {
        g = int(255. - 255. * 6. * (hue - 3./6.));
        b = 0xFF;
    } else if (hue < 5./6.) {
        r = int(255. * 6. * (hue - 4./6.));
        b = 0xFF;
    } else if (hue <= 1.) {
        r = 0xFF;
        b = int(255. - 255. * 6. * (hue - 5./6.));
    } else {
        r = 0; g = 0; b = 0; // Black for a very erroneous input. Should maybe be white?
    }

    return r + (g<<8) + (b<<16);
}

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE

// This function will locate the path to our application on OS X,
// unlike windows you can not rely on the current working directory
// for locating your configuration files and resources.
std::string macBundlePath()
{
    char path[1024];
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    assert(mainBundle);

    CFURLRef mainBundleURL = CFBundleCopyBundleURL(mainBundle);
    assert(mainBundleURL);

    CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
    assert(cfStringRef);

    CFStringGetCString(cfStringRef, path, 1024, kCFStringEncodingASCII);

    CFRelease(mainBundleURL);
    CFRelease(cfStringRef);

    return std::string(path);
}
#endif

} // namespace ap
