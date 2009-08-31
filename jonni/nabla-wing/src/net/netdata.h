#ifndef AP_OOINPUT_NETDATA_H
#define AP_OOINPUT_NETDATA_H

#include <enet/enet.h>
#include <iostream>
#include <string>
#include <map>
#include <list>

#include "NetUser.h"
#include "NetObject.h"
#include "NetEvent.h"
#include "NetMessage.h"
#include "../functions.h"
#include "../types.h"

namespace ap {
namespace net {

using namespace std;
using namespace ap::net;

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
    static const enet_uint8 PACKET_SETAVATAR = 43;
    static const enet_uint8 PACKET_DISCONNECT = 44;
    static const enet_uint8 PACKET_NETMESSAGE = 45;
    static const enet_uint8 PACKET_EOF = 49;

    static const enet_uint8 OBJECT_TYPE_NETGAMEOBJECT = 50;
    static const enet_uint8 OBJECT_TYPE_PROJECTILE = 51;
    static const enet_uint8 OBJECT_TYPE_STARFIELD = 52;
    static const enet_uint8 OBJECT_TYPE_NETUSER = 59;

    static const enet_uint8 EVENT_NOEVENT = 19;
    static const enet_uint8 EVENT_CONNECT = 20;
    static const enet_uint8 EVENT_DISCONNECT = 21;
    static const enet_uint8 EVENT_SETAVATAR = 22;
    static const enet_uint8 EVENT_DELETEOBJECT = 23;
    static const enet_uint8 EVENT_CREATEOBJECT = 24;
    static const enet_uint8 EVENT_MESSAGE = 25;

 private:
    enum status_type { enet_error, offline, connected, server };
    status_type status;
    std::string errorstring;
    ENetHost *enethost;     // this is my own host struct
    ENetPeer *enetserver;   // this is my server's peer struct

    int serviceServer();
    int serviceClient();

    void sendClientChanges();

    std::list <int> objectDeleteQueue;
    std::list <NetEvent *> neteventlist;

    void addEvent(NetEvent *event); // Almost useless, if you can push_back stuff to a list
    uint32 processPacketNetObject(enet_uint8 *data);

 // Were public:
    std::map <int, NetObject*> netobjects;
 public:
    std::map <int, NetUser> users;   // This is, users contacted to US! Should be empty unless we're server.

    NetUser me;
    int myAvatarID;             // if >0, tells which object in the map represents me! (if client)
    void setAvatarID(uint32 uid, uint32 avatarid);

    NetData(int type = NetData::CLIENT, uint32 _port = DEFAULTPORT);
    ~NetData();

    int connect(std::string ip, uint32 port);
    int disconnect();

    int sendChanges();
    int receiveChanges();
    bool pollEvent(NetEvent &event);
    int sendMessage(NetMessage &message);

    NetObject *getNetObject(uint32 id);
    NetObject *eachObject();
    NetObject *eachObject(uint8 type);
    void removeNetObject(uint32 id);

    // Functions below here are meant for server's use. Undefined consequences for clients. Maybe.
    uint32 getUniqueObjectID();
    uint32 insertObject(NetObject *obj);
    void insertObjects(list<NetObject *> *objlist);
    void delObject(uint32 id);

    template <typename ObjType> ObjType getAvatarObject()
    {
        if ((myAvatarID <= 0) || (netobjects.find(myAvatarID) == netobjects.end())) return NULL;
        return dynamic_cast<ObjType>(netobjects.find(myAvatarID)->second);
    }

    template <typename ObjType> ObjType getNetObject(uint32 id)
    {
        std::map <int, NetObject*>::iterator it = netobjects.find(id);
        if( netobjects.end() == it ) return NULL; // return NULL when not found.
        return dynamic_cast<ObjType>(it->second); // maybe cast fails too? Take care.
    }

    template <typename ObjType> ObjType eachObject()
    {
        static std::map<int,NetObject*>::iterator i = netobjects.begin();
        if (i != netobjects.end()) return dynamic_cast<ObjType>((i++)->second);
        i = netobjects.begin();
        return NULL;
    }

    template <typename ObjType> ObjType eachObject(uint8 type)
    {
#define b ,netobjects.begin()
        static std::map<int,NetObject*>::iterator i[256] = { netobjects.begin() b b b b b    b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b    b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b    b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b    b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b    b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b };
#undef b
        while (i[type] != netobjects.end()) {
            if (i[type]->second->getObjectType() == type) return dynamic_cast<ObjType>((i[type]++)->second);
            i[type]++;
        }
        i[type] = netobjects.begin();
        return NULL;
    }

//    int save(string filename);
//    int load(string filename);
};

} // namespace ooinput
} // namespace ap

#endif
