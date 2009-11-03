#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif
#ifdef __APPLE__
#include <SDL/SDL.h>
#else
#include <SDL.h>
#endif

#include <iostream>
#include <cstdlib>  // for random
#include <cmath>    // for sin,cos in the server
#include <list>

#include "net/netdata.h"
#include "netgameobject.h"
#include "starfield.h"
#include "functions.h"
#include "NablaControl.h"

using namespace std;
using namespace ap::net;

int servermain(int argc, char* argv[])
{
    map<int,NetObject *>::iterator p, pProj;
    map<int, NetUser>::iterator iUser;
    NetUser *userp;
    NetData *netdata;
    NetEvent event;
    long int nextupdate;    // When we will send updates to clients the next time
    long int oldticks, newticks;    // These here for tracking time
    float dt;                       //

    cout << "Created a server."<<endl;
    cout << "Serving "<< netobjectprototypes().size()<<" kinds of objects."<<endl;

    netdata = new NetData(NetData::SERVER);
    netdata->insertObject(new StarField());
    newticks = nextupdate = getTicks();

    while (1) {             // Server main loop
        netdata->receiveChanges();
        oldticks = newticks; newticks = getTicks(); dt = float(newticks - oldticks) * 0.001;

	while (NetGameObject *ngop = netdata->eachObject<NetGameObject *>(ap::OBJECT_TYPE_NETGAMEOBJECT))
        {
//JL            iUser = netdata->users.find(ngop->uid);
//JL            if (iUser != netdata->users.end()) // That means the object's owner is still logged in
            userp = netdata->getUser(ngop->uid);
            if (userp) // That means the object's owner is still logged in
            {
                netdata->insertObjects(ngop->control(*userp, true));
            } else {                            // That means owner's disconnected, so paint it gray!
                ngop->color = 0x00666666;
                ngop->uid = 0; // Set owner to zero, so that controls don't go to somebody else
            }

            while (Projectile *pp = netdata->eachObject<Projectile *>(ap::OBJECT_TYPE_PROJECTILE))
            {
                if ((pp) && (pp->loc.collision(ngop->loc))) {
                    ngop->loc.x = 500 + (rand()%4)*1000;    ngop->loc.y = 500 + (rand()%4)*1000;
                    ngop->loc.xvel = ngop->loc.yvel = 0.0;
                    netdata->delObject(pp->id);
                }
            }

        }

        while (NetObject *nop = netdata->eachObject())
            if (nop->advance(dt) == -1) netdata->delObject(nop->id); //removeNetObject(nop->id);

        if (newticks >= nextupdate) {
            netdata->sendChanges();
            nextupdate = newticks + 40; // 40 ms between updates, that's 25 network fps.
        }                               // Seems to me that up to 100 is still okay!
        SDL_Delay(1);       // sleep even a little bit so that dt is never 0

        while (netdata->pollEvent(event)) {
            switch (event.type)
            {
             case NetData::EVENT_CONNECT:
                cout << "Received a connection from "<< uint2ipv4(netdata->getUser(event.uid)->peer->address.host) <<", uid "<<event.uid;
                cout << ". We now have "<<netdata->getUserCount()<<" users."<<endl;

                int newid = netdata->insertObject(new NetGameObject(0, event.uid));
                NetGameObject *newobj = netdata->getObject<NetGameObject*>(newid);
                cout << "Added obj id "<<newid<<" for user "<<event.uid<<", ptr "<<newobj<<endl;
                newobj->controls = new ap::NablaControl();
                netdata->setAvatarID(event.uid, newid);
                netdata->getUser(event.uid)->setControlPtr(newobj->controls);

                break;
            }
        }

    } // Main loop

}
