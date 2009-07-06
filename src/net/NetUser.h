#ifndef AP_NET_NETUSER
#define AP_NET_NETUSER

#include "NetObject.h"
#include <enet/enet.h>
#include <string>
#include <iostream>

namespace ap {
namespace net {

using namespace std;

class NetUser : public NetObject
{
 public:
    static const unsigned int SHOOT_MG = 1;

    ENetPeer *peer;
    int uid;
    std::string nick;

    int color, ping;
    float a, turning;       // m/s², and rad/s
    unsigned int controls;

    NetUser();
    NetUser(int uid, ENetPeer *_peer);

    int serialize(enet_uint8 buffer[], int start, int buflength) const;
    int unserialize(enet_uint8 buffer[], int start);
    NetObject *create(int id);
};

} // namespace net
} // namespace ap

#endif
