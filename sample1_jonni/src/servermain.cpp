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

#include "netdata.h"
#include "netgameobject.h"
#include "functions.h"

using namespace std;


int servermain(int argc, char* argv[])
{
    map<int,NetObject *>::iterator p, pProj;
    map<int, NetUser>::iterator iUser;
    NetData *netdata;
    long int nextupdate;    // When we will send updates to clients the next time
    long int oldticks, newticks;    // These here for tracking time
    float dt;                       //

    cout << "Created a server."<<endl;
    netdata = new NetData(NetData::SERVER);

    cout << "Serving "<< netobjectprototypes().size()<<" objects."<<endl;

    newticks = nextupdate = getTicks();



    while (1) {             // Server main loop
        netdata->receiveChanges();
        oldticks = newticks; newticks = getTicks(); dt = float(newticks - oldticks) * 0.001;

        p = netdata->netobjects.begin();
        while (p != netdata->netobjects.end()) {
            iUser = netdata->users.find(p->second->uid);

            NetGameObject *ptrObj = dynamic_cast<NetGameObject *>(p->second);
            if (ptrObj) // if the dynamic cast succeeded, then p->second is a NetGameObject!
            {
                if (iUser != netdata->users.end()) // That means the object's owner is still logged in
                {
                    vector<NetObject *> *vObjs = ptrObj->control(iUser->second);
                    if (vObjs != NULL) {            // if control returns objects, insert them to netobjects map
                        vector<NetObject *>::iterator ivObj;
                        for (ivObj = vObjs->begin(); ivObj != vObjs->end(); ivObj++)
                        {
                            int newid = netdata->getUniqueObjectID();
                            (*ivObj)->id = newid;
                            netdata->netobjects.insert(make_pair(newid, *ivObj));
                        }
                    }
                } else {                            // That means owner's disconnected, so paint it gray!
                    ptrObj->color = 0x00666666;
                }

                pProj = netdata->netobjects.begin();
                while (pProj != netdata->netobjects.end())
                {
                    Projectile *proj = dynamic_cast<Projectile *>(pProj->second);
                    if ((proj) && (proj->loc.collision(ptrObj->loc))) {
                        ptrObj->loc.x = 0; ptrObj->loc.y = 0;
                        netdata->delObject(pProj->second->id);  // This erases a pair from netobjects map,
                    }                                           // (hope this doesn't do harm),
                    pProj++;                                    // and this still uses an iterator to the one.
                }
            }

            if (p->second->advance(dt) == -1) {
                delete p->second;
                netdata->netobjects.erase(p);
            }

            p++;
        }

        if (newticks >= nextupdate) {
            netdata->sendChanges();
            nextupdate = newticks + 40; // 40 ms between updates, that's 25 network fps.
        }                               // Seems to me that up to 100 is still okay!
        SDL_Delay(1);       // sleep even a little bit so that dt is never 0
    }

}
