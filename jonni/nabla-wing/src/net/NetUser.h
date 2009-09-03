#ifndef AP_NET_NETUSER
#define AP_NET_NETUSER

#include <enet/enet.h>
#include <string>
#include <iostream>

#include "NetObject.h"
#include "Controller.h"
#include "../types.h"

namespace ap {
namespace net {

using namespace std;

class NetUser : public NetObject
{
 public:
    static const unsigned int SHOOT_MG = 1;

    static const enet_uint8 CONTROL_IS_SET = 4;
    static const enet_uint8 CONTROL_NOT_SET = 5;
    static const enet_uint8 CONTROL_BLOCK_FINISHED=6;

    ENetPeer *peer;
    int uid;
    std::string nick;

    int ping;
    Controller *controls;

    NetUser();
    NetUser(int uid, ENetPeer *_peer);
    uint8 getObjectType();

    void setControlPtr(Controller *ctrl);

    int serialize(enet_uint8 buffer[], int start, int buflength) const;
    int unserialize(enet_uint8 buffer[], int start);
    NetObject *create(uint32 id);
};

} // namespace net
} // namespace ap

#endif
