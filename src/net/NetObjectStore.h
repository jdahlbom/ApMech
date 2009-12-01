// WORK IN PROGRESS - NOT IN USE

#ifndef AP_NETOBJECTSTORE_H
#define AP_NETOBJECTSTORE_H

#include <enet/enet.h>
#include <iostream>
#include <string>
#include <map>
#include <list>
#include <set>

#include "NetObject.h"
#include "../functions.h"
#include "../types.h"

namespace ap {
namespace net {

using namespace std;

extern std::map<ap::uint8, NetObject *>& netobjectprototypes();     // This needed here?!

class NetObjectStore {

 private:
    bool __template_eachObject_active[256]; // ONLY FOR templated eachObject.

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

 public:

    NetObjectStore();
    ~NetObjectStore();

    NetObject *getObject(uint32 id);
    NetObject *eachObject();
    void removeObject(uint32 id);

    NetUser *getUser(uint32 uid);
    int getUserCount();
    NetUser *eachUser();

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

} // namespace net
} // namespace ap

#endif
