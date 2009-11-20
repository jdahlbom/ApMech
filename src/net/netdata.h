#ifndef AP_OOINPUT_NETDATA_H
#define AP_OOINPUT_NETDATA_H

#include <enet/enet.h>
#include <iostream>
#include <string>
#include <map>
#include <list>
#include <set>

#include "NetUser.h"
#include "NetObject.h"
#include "NetEvent.h"
#include "NetMessage.h"
#include "../functions.h"
#include "../types.h"

namespace ap {
namespace net {

using namespace std;

map<ap::uint8, NetObject *>& netobjectprototypes();

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
    static const enet_uint8 PACKET_ALERTOBJECT = 46;
    static const enet_uint8 PACKET_EOF = 49;

    static const enet_uint8 EVENT_NOEVENT = 19;
    static const enet_uint8 EVENT_CONNECT = 20;
    static const enet_uint8 EVENT_DISCONNECT = 21;
    static const enet_uint8 EVENT_SETAVATAR = 22;
    static const enet_uint8 EVENT_DELETEOBJECT = 23;
    static const enet_uint8 EVENT_CREATEOBJECT = 24;
    static const enet_uint8 EVENT_MESSAGE = 25;
    static const enet_uint8 EVENT_ALERTOBJECT = 26;

 private:
    enum status_type { enet_error, offline, connected, server };
    status_type status;
    std::string errorstring;
    ENetHost *enethost;     // this is my own host struct
    ENetPeer *enetserver;   // this is my server's peer struct
    bool __template_eachObject_active[256]; // ONLY FOR templated eachObject.

    int serviceServer();
    int serviceClient();

    int sendClientChanges();

    std::set <uint32> objectDeleteQueue;
    std::set <uint32> objectAlertQueue;
    std::list <NetEvent *> neteventlist;

    void addEvent(NetEvent *event); // Almost useless, if you can push_back stuff to a list
    uint32 processPacketNetObject(enet_uint8 *data);
    ap::uint32 createPacketNetObject(const ap::net::NetObject* pObj, enet_uint8 *data,
				     ap::uint32 start, ap::uint32 buflength) const;

    std::map <int, NetObject*> netobjects;
    std::multimap <ap::uint8, NetObject*> netObjectsByType; // Index table for efficient eachObject
    std::map <int, NetUser> users;   // This is, users contacted to US! Should be empty unless we're server.
    std::map <int, NetUser> pastUsers;// Users who disconnected already. A non-strict storage

 public:

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

    NetObject *getObject(uint32 id);
    NetObject *getFirstObjectOfType(uint8 type);
    NetObject *eachObject();
    void removeObject(uint32 id);

    NetUser *getUser(uint32 uid);
    NetUser *getPastUser(uint32 uid);
    int getUserCount();
    NetUser *eachUser();

    template <typename ObjType> ObjType getAvatarObject() {
        if ((myAvatarID <= 0) || (netobjects.find(myAvatarID) == netobjects.end())) return NULL;
        return dynamic_cast<ObjType>(netobjects.find(myAvatarID)->second);
    }

    template <typename ObjType> ObjType getObject(uint32 id) {
        std::map <int, NetObject*>::iterator it = netobjects.find(id);
        if( netobjects.end() == it ) return NULL; // return NULL when not found.
        return dynamic_cast<ObjType>(it->second); // maybe cast fails too? Take care.
    }

    template <typename ObjType> ObjType eachObject(uint8 type) {
        static pair<multimap<uint8,NetObject*>::iterator, multimap<uint8,NetObject*>::iterator> ii[256];

        if (false == __template_eachObject_active[type]) {
            ii[type] = netObjectsByType.equal_range(type);
            __template_eachObject_active[type] = true;
        }

        if (ii[type].first == ii[type].second) {    // We're at the end!
            __template_eachObject_active[type] = false;
            return NULL;
        } else {                        // Not at the end yet!
            return dynamic_cast<ObjType>((ii[type].first++)->second);
        }
    }

// Functions below here are meant for server's use. Undefined consequences for clients. Maybe.
    uint32 getUniqueObjectID();
    uint32 insertObject(NetObject *obj, uint32 id = 0);
    void insertObjects(list<NetObject *> *objlist);
    void delObject(uint32 id);
    void alertObject(uint32 id);

//    int save(string filename);
//    int load(string filename);
};

} // namespace ooinput
} // namespace ap

#endif
