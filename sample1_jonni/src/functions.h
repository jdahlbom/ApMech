/*  Random small functions
 *
 *  Jonni Lehtiranta
 */

#include <string>
#include <sstream>
#include <iostream>
#include <enet/enet.h>

using namespace std;

string uint2ipv4(unsigned int ip);  // Convert a uint32 to a string with the corresponding IP address
void hexprint(enet_uint8 *buffer, int length);
