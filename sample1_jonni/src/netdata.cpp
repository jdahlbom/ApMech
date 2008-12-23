#include "netdata.h"

NetData::NetData(int type, int _port) {
    if (enet_initialize() != 0) {
        status = enet_error;
        errorstring = "Error in enet_initialize()";
    } else if (type == NetData::SERVER) {
        status = server;
        me.peer.address.host = ENET_HOST_ANY; me.peer.address.port = _port;
        enethost = enet_host_create(&me.peer.address, MAXCLIENTS+1, 0, 0); // Assuming any in/out bandwidth
        me.uid = 0; // The server is Net-UID 0!
    } else if (type == NetData::CLIENT) {
        status = offline;
        enethost = enet_host_create(NULL, 1, 0, 0); // OBS! Here too, no bandwidth-guessing
    } else status = offline;

    if (enethost == NULL) {
        status = enet_error;
        errorstring = "Error using enet_host_create in NetData constructor";
    }

    me.uid = -1; enetserver = NULL;
    return;
}

NetData::~NetData() {
    if (status == connected) disconnect();
    enet_deinitialize();
    return;
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
        return ERROR;
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
                cout << "I received an UID! It is "<<me.uid<<endl;
                enet_packet_destroy(event.packet);
                return 1;
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


int NetData::service()
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
                users.insert(make_pair(newuid, NetUser(newuid, *event.peer)));

                ENetPacket *packet = enet_packet_create(&newuid, 4, ENET_PACKET_FLAG_RELIABLE); // OBS! Bad way to do this!
                enet_peer_send(event.peer, 0, packet);
                cout << "Received a connection from "<< uint2ipv4(event.peer->address.host) <<", uid "<<newuid<<endl;
            }
            break;

         case ENET_EVENT_TYPE_RECEIVE:
            if (status == server) {                      // Server action. Update a user
                enet_uint8 *data = event.packet->data;
                int parts = data[0], h=1;
                int uid;
                for (int i=0; i<parts; i++)
                {
                    if (data[h] == NetData::PACKET_NETUSER) {
                        h++; uid = *(int *)(data+h);  // this means, read an int from data[h] onwards
                        h += users.find(uid)->second.serialset(data, h);
                    }
                }
                enet_packet_destroy(event.packet);

            } else if (status == connected) {        // We're a user, server's sending us stuff!
                enet_uint8 *data = event.packet->data;
                int parts = data[0], h=1;
                int uid;
                for (int i=0; i<parts; i++)
                {
                    if (data[h] == NetData::PACKET_NETUSER) {
                        h++; uid = *(int *)(data+h);  // this means, read an int from data[h] onwards
                        if (users.find(uid) != users.end()) h += users.find(uid)->second.serialset(data, h);
                        else {
                            users.insert(make_pair(uid, NetUser(uid, *event.peer)));
                            h += users.find(uid)->second.serialset(data, h);
                        }
                    } else if (data[h] == NetData::PACKET_DISCONNECT) {
                        h++; uid = *(int *)(data+h);
                        cout << users.find(uid)->second.nick << " (uid "<<uid<<") has disconnected."<<endl;
                        users.erase(uid);
                    }
                }
                enet_packet_destroy(event.packet);
            }
            break;

         case ENET_EVENT_TYPE_DISCONNECT:
            if (status == connected) {      // that means, a client loses connection unwillingly
                me.uid = -1; status = offline;
                if (int(event.data) == NetData::SERVERFULL) cout << "Server full, connection closed!" << endl;
                else cout << "Connection lost!" << endl;
            } else if (status == server) {
                users.erase(int(event.data));
                cout << "Client " << event.data << " has disconnected" << endl;

                enet_uint8 buffer[6]; buffer[0] = 1; buffer[1] = NetData::PACKET_DISCONNECT;    // Let clients know!
                *(int *)(buffer + 2) = int(event.data);
                ENetPacket *packet = enet_packet_create(buffer, 6, ENET_PACKET_FLAG_RELIABLE);
                enet_host_broadcast(enethost, 0, packet);
                // Not detecting if we get disconnect messages from users who weren't connected in the first place. Oh well.
            }
            break;
         case ENET_EVENT_TYPE_NONE:
            break; // What should we do? What events are of the NONE type?
        }
        items++;
    }



    if ((status == connected) && (me.changed)) { // Do we have updated things to send!

        enet_uint8 buffer[10000];
        int length=1;

        buffer[length++] = NetData::PACKET_NETUSER;
        length += me.serialize(buffer, length, 10000);
        buffer[0] = 1;

        ENetPacket *packet = enet_packet_create(buffer, length, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(enetserver, 0, packet);
        me.changed = false;

    } else if ((status == server) ) {          // 1st iteration: send anyway, even if no updates.

        enet_uint8 buffer[10000];
        int length=1, userstosend = 0;
        map<int,NetUser>::iterator p = users.begin();

        while (p != users.end()) {
            buffer[length++] = NetData::PACKET_NETUSER;
            length += p->second.serialize(buffer, length, 10000);
            userstosend++; p++;
        }
        buffer[0] = userstosend;

        if (userstosend > 0) {
            ENetPacket *packet = enet_packet_create(buffer, length, ENET_PACKET_FLAG_RELIABLE);
            enet_host_broadcast(enethost, 0, packet);
        }

//        me.changed = false;// Is server's "me" even used like this?!?
    }

    return items;
}
