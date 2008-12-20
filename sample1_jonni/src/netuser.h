#include <enet/enet.h>
#include <string>
#include <iostream>

using namespace std;

class NetUser
{
 public:
    bool changed;

    ENetPeer peer;
    int uid;
    std::string nick;

    int x, y, color;

    NetUser();
    NetUser(int uid, ENetPeer _peer);

    int serialize(enet_uint8 buffer[], int start, int buflength);
    int serialset(enet_uint8 buffer[], int start);
};
