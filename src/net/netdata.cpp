#include "netdata.h"

namespace ap {
namespace net {

using namespace std;


map<enet_uint8, NetObject *>& netobjectprototypes()
{
    static map<enet_uint8, NetObject *> * objectprotomap = new map<enet_uint8, NetObject *>;
    return *objectprotomap;
}

void NetData::addEvent(NetEvent *event)
{
    neteventlist.push_back(event);
}
bool NetData::pollEvent(NetEvent *event)
{
    if (!neteventlist.empty()) {
        *event = *neteventlist.front(); // Copy what's in there
        delete neteventlist.front();
        neteventlist.pop_front();
        return true;
    } else return false;
}

NetData::NetData(int type, int _port) {
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

    std::map<int, NetObject *>::iterator objIterator;
    for(objIterator = netobjects.begin(); objIterator!=netobjects.end(); ++objIterator) {
        delete(objIterator->second);
    }
    netobjects.erase(netobjects.begin(), netobjects.end());
    return;
}

void NetData::setAvatarID(int uid, int avatarid)
{
    enet_uint8 buffer[6];
    buffer[0] = NetData::PACKET_SETAVATAR;
    *(int *)(buffer+1) = avatarid; buffer[5] = NetData::PACKET_EOF;
    ENetPacket *packet = enet_packet_create(buffer, 6, ENET_PACKET_FLAG_RELIABLE);
    map<int,NetUser>::iterator userIterator;
    userIterator = users.find(uid);
    if (userIterator == users.end()) cout << "FOULED in NetData::setAvatarID: Uid not found!"<<endl;
    else enet_peer_send(userIterator->second.peer, 0, packet);
}

int NetData::connect(std::string ip, int port)
{
    ENetAddress address;
    ENetEvent event;

    enet_address_set_host(&address, ip.c_str());
    address.port = port;
    enetserver = enet_host_connect(enethost, &address, 1);

    if (enetserver == NULL) {
        cout << (errorstring = "Connect to server failed") << endl;
        return NETERROR;
    }

    if ((enet_host_service(enethost, &event, 5000) > 0) && (event.type == ENET_EVENT_TYPE_CONNECT))
    {
        cout << "Connected to server!"<<endl;
        status = connected;
    } else {
        enet_peer_reset(enetserver);
        errorstring = "Connect to server failed: timeout error";
        cout << errorstring << endl;
        return TIMEOUT;
    }

    while (enet_host_service(enethost, &event, 3000) > 0) {
        switch (event.type) {
         case ENET_EVENT_TYPE_RECEIVE:
            if (event.packet->dataLength == 4) {  // Ha! It seems we just connected, and now we get our UID.
                me.uid = int(*event.packet->data);
                cout << "I received an UID! It is "<<me.uid<<"."<<endl;
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
                cout << "Disconnected from server successfully" << endl;
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
                    } else {
                        cout << "Received an unknown packet! Error in NetData::serviceServer!" << endl;
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
                delete (int *)event.peer->data;     // I wonder why I wrote this line // not anymore!
                cout << "Client " << uid << " has disconnected" << endl;


                for (map<int, NetObject *>::iterator iObj = netobjects.begin() ; iObj != netobjects.end() ; iObj++)
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
    enet_uint8 *data, objtype;
    int items = 0, h, uid, id;

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
                    h++;
                    id = *(int *)(data+h);  // this means, read an int from data[h] onwards
                    if (netobjects.find(id) != netobjects.end()) {
                        h+= netobjects.find(id)->second->unserialize(data, h);
                    } else {
                        objtype = *(enet_uint8 *)(data+h+4);
                        netobjects.insert(make_pair(id, ap::net::netobjectprototypes()[objtype]->create(id)));
                        h += netobjects.find(id)->second->unserialize(data, h);
                        addEvent(new NetEvent(EVENT_CREATEOBJECT, id));
                    }
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
                } else if (data[h] == NetData::PACKET_DISCONNECT) {
                    h++;
                    uid = *(int *)(data+h);
                    addEvent(new NetEvent(EVENT_DISCONNECT, 0));
                    h+=4;
                    cout << "Player "<<uid<<" disconnected!"<<endl;
                    // TODO: DO SOMETHING TO INDICATE THIS!
                    // That, in addition to adding some class for player info, to give users
                } else {
                    cout << "Received an unknown packet! Error in NetData::serviceClient!" << endl;
                    h++;
                }
            }
            enet_packet_destroy(event.packet);
            break;}

         case ENET_EVENT_TYPE_DISCONNECT:   // that means, a client loses connection unwillingly
            me.uid = -1; status = offline;
            if (int(event.data) == NetData::SERVERFULL) cout << "Server full, connection closed!" << endl;
            else cout << "Connection lost!" << endl;
            break;

         case ENET_EVENT_TYPE_CONNECT:      // Nobody should connect a client!
         case ENET_EVENT_TYPE_NONE:         // What should we do? What events are of the NONE type?
            break;
        }
        items++;
    }
    return items;
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
    int items = 0;
    if ((status == connected) && (me.changed) && (me.uid > 0)) { // Do we have updated things to send!
        sendClientChanges();
    } else if ((status == server) ) {          // 1st iteration: send anyway, even if no updates.
        enet_uint8 buffer[10000];
        int length=0, packetstosend = 0;
        map<int,NetObject *>::iterator po = netobjects.begin();
        list<int>::iterator iDelPkg = objectDeleteQueue.begin();

        while (iDelPkg != objectDeleteQueue.end()) {
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

        if (packetstosend > 0) {
            ENetPacket *packet = enet_packet_create(buffer, length, 0);//ENET_PACKET_FLAG_RELIABLE);
            enet_host_broadcast(enethost, 0, packet);
        }

//        me.changed = false;// Is server's "me" even used like this?!?
    }

    return items;
}

int NetData::getUniqueObjectID()
{
    int newid;
    for (newid = 1; netobjects.find(newid) != netobjects.end(); newid++); // and first unused object id
    return newid;
}

NetObject * NetData::getNetObject(int id)
{
    netObjectsType::iterator it = netobjects.find(id);
    if( netobjects.end() == it ) {
        // TODO: Should throw an exception if no object found! Or return null?
    }
    return it->second;
}

// Client side delete
void NetData::removeNetObject(int id)
{
    NetObject *removable = netobjects.find(id)->second;
    delete removable;
    netobjects.erase(id); // TODO: Shouldn't erase call the destructor anyways??
}

// Server delete
void NetData::delObject(int id)
{
    objectDeleteQueue.push_back(id);
    delete netobjects.find(id)->second;
    netobjects.erase(id);
}

} //namespace net
} //namespace ap
