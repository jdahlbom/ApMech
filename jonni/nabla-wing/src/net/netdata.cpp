#include "netdata.h"

#include "../types.h"
#include "serializer.h"

namespace ap {
namespace net {

using namespace std;


map<enet_uint8, NetObject *>& netobjectprototypes()
{
    static map<enet_uint8, NetObject *> * objectprotomap = new map<enet_uint8, NetObject *>;
    return *objectprotomap;
}

NetData::NetData(int type, uint32 _port) :
    neteventlist(std::list <NetEvent *>())
{
    if (enet_initialize() != 0) {
        status = enet_error;
        errorstring = "Error in enet_initialize()";
    } else if (type == NetData::SERVER) {
        status = server;
        me.peer = new ENetPeer();
        me.peer->address.host = ENET_HOST_ANY; me.peer->address.port = _port;
        enethost = enet_host_create(&me.peer->address, MAXCLIENTS+1, 0, 0); // Assuming any in/out bandwidth
        me.uid = 0; // The server is Net-UID 0!
    } else if (type == NetData::CLIENT) {
        status = offline;
        enethost = enet_host_create(NULL, 1, 0, 0); // OBS! Here too, no bandwidth-guessing
    } else status = offline;

    if (enethost == NULL) {
        status = enet_error;
        errorstring = "Error using enet_host_create in NetData constructor";
    }
    me.uid = -1; myAvatarID = 0; enetserver = NULL;
    return;
}

NetData::~NetData() {
    if (status == connected) disconnect();
    enet_deinitialize();

    std::map <int, NetObject*>::iterator objIterator;
    for(objIterator = netobjects.begin(); objIterator!=netobjects.end(); ++objIterator) {
        delete(objIterator->second);
    }
    netobjects.erase(netobjects.begin(), netobjects.end());
    return;
}

void NetData::addEvent(NetEvent *event)
{
    neteventlist.push_back(event);
}
bool NetData::pollEvent(NetEvent &event)
{
    if (!neteventlist.empty()) {
//        cout << "event1message: "<<event.message<<","<<(*neteventlist.front()).message<<endl;
        event = *neteventlist.front(); // Copy what's in there
        delete neteventlist.front();
        neteventlist.pop_front();
        return true;
    } else return false;
}
int NetData::sendMessage(NetMessage &message)
{
    int length = 0;
    enet_uint8 buffer[2000];
    buffer[length++] = NetData::PACKET_NETMESSAGE;
    length += message.serialize(buffer, length, 2000-length);
    buffer[length++] = NetData::PACKET_EOF;
    hexprint(buffer, length); // DEBUG
    ENetPacket *packet = enet_packet_create(buffer, length, ENET_PACKET_FLAG_RELIABLE);
    enet_host_broadcast(enethost, 0, packet);   // Broadcast always. Hub, not switch. For now.
    return length;
}


void NetData::setAvatarID(uint32 uid, uint32 avatarid)
{
    std::cout << "[NETDATA] setAvatarId(" << uid << ", " << avatarid << ") sent." << std::endl;
    std::cout << "[NETDATA] -> " << netobjects.size() << " objects in map." << std::endl;
    enet_uint8 buffer[6];
    buffer[0] = NetData::PACKET_SETAVATAR;
    *(int *)(buffer+1) = avatarid; buffer[5] = NetData::PACKET_EOF;
    ENetPacket *packet = enet_packet_create(buffer, 6, ENET_PACKET_FLAG_RELIABLE);
    map<int,NetUser>::iterator userIterator;
    userIterator = users.find(uid);
    if (userIterator == users.end()) cout << "FOULED in NetData::setAvatarID: Uid not found!"<<endl;
    else enet_peer_send(userIterator->second.peer, 0, packet);
}

int NetData::connect(std::string ip, uint32 port)
{
    ENetAddress address;
    ENetEvent event;

    enet_address_set_host(&address, ip.c_str());
    address.port = port;
    enetserver = enet_host_connect(enethost, &address, 1);

    if (enetserver == NULL) {
        cout << (errorstring = "[NETDATA] Connect to server failed") << endl;
        return NETERROR;
    }

    if ((enet_host_service(enethost, &event, 5000) > 0) && (event.type == ENET_EVENT_TYPE_CONNECT))
    {
        cout << "[NETDATA] Connected to server!"<<endl;
        status = connected;
    } else {
        enet_peer_reset(enetserver);
        errorstring = "[NETDATA] Connect to server failed: timeout error";
        cout << errorstring << endl;
        return TIMEOUT;
    }

    while (enet_host_service(enethost, &event, 3000) > 0) {
        switch (event.type) {
         case ENET_EVENT_TYPE_RECEIVE:
            if (event.packet->dataLength == 4) {  // Ha! It seems we just connected, and now we get our UID.
                me.uid = int(*event.packet->data);
                cout << "[NETDATA] I received an UID! It is "<<me.uid<<"."<<endl;
                enet_packet_destroy(event.packet);
                return 1;
            } else {
                cout << "FOULED in NetData::connect! Bad or wrong packet received after connection!"<<endl;
                enet_packet_destroy(event.packet);
            }
         default:
            break;
        }
    }
    return 0;
}


int NetData::disconnect()
{
    ENetEvent event;

    if (status == connected)
    {
        enet_peer_disconnect(enetserver, me.uid);

        while (enet_host_service(enethost, &event, 3000) > 0) {
            switch (event.type) {
             case ENET_EVENT_TYPE_DISCONNECT:
                cout << "[NETDATA] Disconnected from server successfully" << endl;
                return 1;
                break;
             default:       // We're only interested in disconnect events
                enet_packet_destroy(event.packet);
                break;
            }
        }
        enet_peer_reset(enetserver); // Clean disconnect didn't work - do it by force
        return 0;
    }
    return 0;
}


int NetData::serviceServer()
{
    ENetEvent event;
    int items = 0;

    while (enet_host_service(enethost, &event, 0))
    {
        switch (event.type)
        {
         case ENET_EVENT_TYPE_CONNECT:          // I guess we're the server. There's a new connection! Lotsa work.
            if (users.size() == MAXCLIENTS) {   // If server is full..
                enet_peer_disconnect(event.peer, NetData::SERVERFULL);
            } else {
                int newuid;
                for (newuid = 1; users.find(newuid) != users.end(); newuid++); // find first unused uid
                users.insert(make_pair(newuid, NetUser(newuid, event.peer)));
                event.peer->data = new int(newuid);

                enet_uint8 buffer[4]; *(int *)(buffer) = newuid;
                ENetPacket *packet = enet_packet_create(buffer, 4, ENET_PACKET_FLAG_RELIABLE); // OBS! Bad way to do this!
                enet_peer_send(event.peer, 0, packet);

                addEvent(new NetEvent(NetData::EVENT_CONNECT, newuid));
            }
            break;

         case ENET_EVENT_TYPE_RECEIVE:
            if (status == server) {                      // Server action. Update a user
                enet_uint8 *data = event.packet->data;
                int uid, h=0;
                while (data[h] != NetData::PACKET_EOF)
                {
                    if (data[h] == NetData::PACKET_NETUSER) {
                        h++; uid = *(int *)(data+h);  // this means, read an int from data[h] onwards
                        h += users.find(uid)->second.unserialize(data, h);
                    } else if (data[h] == NetData::PACKET_NETMESSAGE) {
                        NetMessage netmsg;
                        h++;
                        h += netmsg.unserialize(data, h);
                        sendMessage(netmsg);
                    } else {
                        cout << "[NETDATA] Received an unknown packet! Error in NetData::serviceServer!" << endl;
                    }
                }
                enet_packet_destroy(event.packet);
            }
            break;

         case ENET_EVENT_TYPE_DISCONNECT:
            if (status == server) {
                int uid = *(int *)event.peer->data;     // Who disconnected? He sent his uid too,
                                                        // but rather trust enet's peer->data
                users.erase(uid);
                delete (int *)event.peer->data;
                cout << "[NETDATA] Client " << uid << " has disconnected" << endl;

                std::map <int, NetObject*>::iterator iObj;
                for (iObj=netobjects.begin() ; iObj!=netobjects.end() ; iObj++)
                    if (iObj->second->uid == uid) iObj->second->uid = -1;

                enet_uint8 buffer[6]; buffer[0] = NetData::PACKET_DISCONNECT;    // Let clients know!
                *(int *)(buffer + 1) = uid;
                buffer[5] = NetData::PACKET_EOF;
                ENetPacket *packet = enet_packet_create(buffer, 6, ENET_PACKET_FLAG_RELIABLE);
                enet_host_broadcast(enethost, 0, packet);
                // Not detecting if we get disconnect messages from users who weren't connected in the first place. Oh well.

                addEvent(new NetEvent(NetData::EVENT_DISCONNECT, uid));
            }
            break;
         case ENET_EVENT_TYPE_NONE:
            break; // What should we do? What events are of the NONE type?
        }
        items++;
    }
    return items;
}

int NetData::serviceClient()
{
    ENetEvent event;
    enet_uint8 *data;
    int items = 0, h;
    uint32 uid, id;

    while (enet_host_service(enethost, &event, 0))
    {
        switch (event.type)
        {

         case ENET_EVENT_TYPE_RECEIVE:{        // We're a user, server's sending us stuff!
            data = event.packet->data;
            h=0;
            while (data[h] != NetData::PACKET_EOF)
            {
                if (data[h] == NetData::PACKET_NETOBJECT) {
                    ++h;
                    h += processPacketNetObject(data+h);
                } else if (data[h] == NetData::PACKET_DELOBJECT) {
                    h++;
                    id = *(int *)(data+h);
                    addEvent(new NetEvent(EVENT_DELETEOBJECT, id));
                    h += 4;
                } else if (data[h] == NetData::PACKET_SETAVATAR) {
                    h++;
                    myAvatarID = *(int *)(data+h);
                    addEvent(new NetEvent(EVENT_SETAVATAR, myAvatarID));
                    h += 4;
                } else if (data[h] == NetData::PACKET_NETMESSAGE) {
                    NetMessage *netmsg = new NetMessage();
                    h++;
                    h += netmsg->unserialize(data, h);
                    NetEvent *event = new NetEvent(NetData::EVENT_MESSAGE, netmsg);
                    addEvent(event);
                    cout << "MESSAGE: "<<netmsg->data<<endl;
                } else if (data[h] == NetData::PACKET_DISCONNECT) {
                    h++;
                    uid = *(int *)(data+h);
                    addEvent(new NetEvent(EVENT_DISCONNECT, 0));
                    h+=4;
                    cout << "[NETDATA] Player "<<uid<<" disconnected!"<<endl;
                    // TODO: DO SOMETHING TO INDICATE THIS!
                    // That, in addition to adding some class for player info, to give users
                } else {
                    cout << "[NETDATA] Received an unknown packet! Error in NetData::serviceClient!" << endl;
                    h++;
                }
            }
            enet_packet_destroy(event.packet);
            break;}

         case ENET_EVENT_TYPE_DISCONNECT:   // that means, a client loses connection unwillingly
            me.uid = -1; status = offline;
            if (int(event.data) == NetData::SERVERFULL) cout << "[NETDATA] Server full, connection closed!" << endl;
            else cout << "[NETDATA] Connection lost!" << endl;
            break;

         case ENET_EVENT_TYPE_CONNECT:      // Nobody should connect a client!
         case ENET_EVENT_TYPE_NONE:         // What should we do? What events are of the NONE type?
            break;
        }
        ++items;
    }

    return items;
}

uint32 NetData::processPacketNetObject(enet_uint8 *data)
{
    ap::uint8 objtype;
    ap::uint32 id;

    int length = 0;
    length = ap::net::unserialize(id, data, length);

    if (netobjects.find(id) != netobjects.end()) {
        length = netobjects.find(id)->second->unserialize(data, 0);
    } else {
        length += ap::net::unserialize(objtype, data, length);
        netobjects.insert(make_pair(id, ap::net::netobjectprototypes()[objtype]->create(id)));
        length = netobjects.find(id)->second->unserialize(data, 0);
        addEvent(new NetEvent(EVENT_CREATEOBJECT, id));
    }

    return length;
}

int NetData::receiveChanges()
{
    int items = 0;
    if (status == server) items += serviceServer();
    else if (status == connected) items += serviceClient();
    return items;
}

void NetData::sendClientChanges()
{
    enet_uint8 buffer[10000];
    int length=0;

    buffer[length++] = NetData::PACKET_NETUSER;
    length += me.serialize(buffer, length, 10000);
    buffer[length++] = NetData::PACKET_EOF;

    ENetPacket *packet = enet_packet_create(buffer, length, 0);//ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(enetserver, 0, packet);
    me.changed = false;
}

int NetData::sendChanges()
{
    //std::cout << "[NETDATA] Sending changes - " << netobjects.size() << " objects in map" << std::endl;
    int items = 0;
    if ((status == connected) && (me.changed) && (me.uid > 0)) { // Do we have updated things to send!
        list<int>::iterator iDelPkg = objectDeleteQueue.begin();
        while (iDelPkg != objectDeleteQueue.end()) {
            removeNetObject(*iDelPkg);    // First, remove what is to be removed
            iDelPkg = objectDeleteQueue.erase(iDelPkg);
        }

        sendClientChanges();
    } else if ((status == server) ) {          // 1st iteration: send anyway, even if no updates.
        enet_uint8 buffer[10000];
        int length=0, packetstosend = 0;
        std::map <int, NetObject*>::iterator po = netobjects.begin();
        list<int>::iterator iDelPkg = objectDeleteQueue.begin();

        while (iDelPkg != objectDeleteQueue.end()) {
            removeNetObject(*iDelPkg);    // Now, actually remove an object
            buffer[length++] = NetData::PACKET_DELOBJECT;
            *(int *)(buffer+length) = *iDelPkg;     length += 4;
            iDelPkg = objectDeleteQueue.erase(iDelPkg);
        }
        po = netobjects.begin();
        while (po != netobjects.end()) {
            buffer[length++] = NetData::PACKET_NETOBJECT;
            length += po->second->serialize(buffer, length, 10000);
            packetstosend++; po++;
        }
        buffer[length++] = NetData::PACKET_EOF;

/* // Debug info: print what we see in map, and how it's serialized
for (map<int,NetObject*>::iterator i=netobjects.begin() ; i != netobjects.end() ; i++)
    cout << "Map ID "<<i->first << " to "<<i->second->id<<endl;
hexprint(buffer, length);
*/
        if (packetstosend > 0) {
            ENetPacket *packet = enet_packet_create(buffer, length, 0);//ENET_PACKET_FLAG_RELIABLE);
            enet_host_broadcast(enethost, 0, packet);
        }

//        me.changed = false;// Is server's "me" even used like this?!?
    }

    return items;
}

uint32 NetData::getUniqueObjectID()
{
    uint32 newid;
    for (newid = 1; netobjects.find(newid) != netobjects.end(); newid++); // and first unused object id
    return newid;
}

NetObject * NetData::getNetObject(uint32 id)
{
    std::map <int, NetObject*>::iterator it = netobjects.find(id);
    if( netobjects.end() == it ) {
        // TODO: Should throw an exception if no object found! Or return null?
        return NULL;    // Well, rather return null than netobjects.end() for now -- JL
    }
    return it->second;
}

// Client side delete. Just delete, don't let anyone know.
void NetData::removeNetObject(uint32 id)
{
    // I had nabla-wing crash once because of double-delete, but couldn't reproduce..
    // checking doesn't harm anyway.
    if (netobjects.find(id) != netobjects.end()) {
        delete netobjects.find(id)->second;
        netobjects.erase(id); // TODO: Shouldn't erase call the destructor anyways??
    }
}

NetObject *NetData::eachObject()
{
    static std::map<int,NetObject*>::iterator i = netobjects.begin();

    if (i != netobjects.end()) {
        return (i++)->second;
    } else {
        i = netobjects.begin();
        return NULL;
    }
}

NetObject *NetData::eachObject(uint8 type)
{
#define b ,netobjects.begin()
    static std::map<int,NetObject*>::iterator i[256] = { netobjects.begin() b b b b b
    b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b
    b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b
    b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b
    b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b
    b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b };
#undef b
    while (i[type] != netobjects.end()) {
        if (i[type]->second->getObjectType() == type) return (i[type]++)->second;
        i[type]++;
    }
    i[type] = netobjects.begin();
    return NULL;
}

/** Insert an object and give it a new, unique ID.
 *  \return the new, unique ID given to the object. */
uint32 NetData::insertObject(NetObject *obj)
{
    uint32 newid = getUniqueObjectID();
    obj->id = newid;
    netobjects.insert(make_pair(newid, obj));
    return newid;
}

/** Insert several objects at once, and give each of them
 *  a new unique ID. */
void NetData::insertObjects(list<NetObject *> *objlist)
{
    if (objlist != NULL) {            // if control returns objects, insert them to netobjects map
        list<NetObject *>::iterator i;
        for (i = objlist->begin(); i != objlist->end(); i++)
            insertObject(*i);
    }
}

// Server delete (let clients know as well)
void NetData::delObject(uint32 id)
{
    objectDeleteQueue.push_back(id);
//    removeNetObject(id);    // Let's not remove yet, lest we fuck up iterators
//    delete netobjects.find(id)->second;
//    netobjects.erase(id);
}

} //namespace net
} //namespace ap