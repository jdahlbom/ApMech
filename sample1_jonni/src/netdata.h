#ifndef __netdata_h__
#define __netdata_h__

#include <enet/enet.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "netuser.h"
#include "netobject.h"
#include "functions.h"

#include "netgameobject.h"  // Temporary solution. Optimally, netdata core sources shouldn't
                            // have to know about served objects.

using namespace std;

map<enet_uint8, NetObject *>& netobjectprototypes();

class NetData {
 public:
                // Settings
    static const unsigned int DEFAULTPORT = 5074;
    static const unsigned int MAXCLIENTS = 32;

                // Tags
    static const int CLIENT = 0;
    static const int SERVER = 1;

    static const int NETERROR = -1;     // This can't be "ERROR". Doesn't compile on windows mingw if it is.
    static const int TIMEOUT = -2;
    static const int SERVERFULL = -3;

    static const enet_uint8 PACKET_NETUSER = 40;
    static const enet_uint8 PACKET_NETOBJECT = 41;
    static const enet_uint8 PACKET_DELOBJECT = 42;
    static const enet_uint8 PACKET_DISCONNECT = 9;

    static const enet_uint8 OBJECT_TYPE_NETGAMEOBJECT = 50;
    static const enet_uint8 OBJECT_TYPE_PROJECTILE = 51;
    static const enet_uint8 OBJECT_TYPE_STARFIELD = 52;

 private:
    enum status_type { enet_error, offline, connected, server };
    status_type status;
    std::string errorstring;
    ENetHost *enethost;     // this is my own host struct
    ENetPeer *enetserver;   // this is my server's peer struct

    int serviceServer();
    int serviceClient();

 public:
    map <int, NetUser> users;   // This is, users contacted to US! Should be empty unless we're server.
    map <int, NetObject *> netobjects;
    vector <int> objectDeleteQueue;

    NetUser me;
    int myAvatarID;             // if >0, tells which object in the map represents me!

    NetData(int type = NetData::CLIENT, int _port = DEFAULTPORT);
    ~NetData();

    int connect(std::string ip, int port);
    int disconnect();

    int sendChanges();
    int receiveChanges();

    // Functions below here are meant for server's use. Undefined consequences for clients. Maybe.

    int getUniqueObjectID();
    void delObject(int id);
};


#endif
