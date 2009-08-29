/*  Random small functions
 *
 *  Jonni Lehtiranta
 */

using namespace std;

#include <string>
#include <sstream>
#include <iostream>
#include <enet/enet.h>
#include <sys/time.h>
#include <cmath>
#include <list>
#include <SDL.h>
#include "SDL_ttf.h"
#include "net/NetObject.h"
#include "netgameobject.h"
#include "projectile.h"
#include "starfield.h"

string uint2ipv4(unsigned int ip);  // Convert a uint32 to a string with the corresponding IP address

void hexprint(enet_uint8 *buffer, int length); // Cout a random buffer and maybe understand what's in it!

template <class T> inline std::string to_string (const T &t); // generic to_string() for any pipeable thing

long int getTicks(); // milliseconds since approx program start. OBS! Not C-standard, but POSIX

// Draw helper functions
void putpixel(SDL_Surface *s, int x, int y, int r, int g, int b);
void putline2(SDL_Surface *s, int _x, int _y, int _x2, int _y2, int r, int g, int b);
void puttriangle(SDL_Surface *s, int x, int y, int x2, int y2, int x3, int y3, int r, int g, int b);
void puttriangle_c(SDL_Surface *s, int x, int y, int x2, int y2, int x3, int y3, int r, int g, int b);
void putsputnik(SDL_Surface *s, int x, int y, int size, float angle, int r, int g, int b);

int drawObject(SDL_Surface *s, float x, float y, ap::net::NetObject *no);

int drawTextArea(SDL_Surface *s, float x, float y, const std::string &content, TTF_Font *font);
