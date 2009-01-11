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

int main ( int argc, char** argv )
{

    SDL_Surface *screen;
    SDL_Rect dstrect;
    NetData *netdata;

    srand(time(NULL));

    if ((argc == 2) && (strcmp(argv[1], "-server") == 0)) { // ********* SERVER CODE
        map<int,NetObject *>::iterator p;
        NetUser *ptrUser;
        long int oldticks, newticks;
        float dt;

        cout << "Created a server."<<endl;
        netdata = new NetData(NetData::SERVER);

        cout << "Serving "<< netobjectprototypes().size()<<" objects."<<endl;

        newticks = getTicks();

        while (1) {             // Server main loop
            netdata->service();
            oldticks = newticks; newticks = getTicks(); dt = float(newticks - oldticks) * 0.001;

            p = netdata->netobjects.begin();
            while (p != netdata->netobjects.end()) {
                ptrUser = &netdata->users[p->second->uid];

                if (typeid(*p->second) == typeid(NetGameObject)) {
                    NetGameObject *ptrObj = (NetGameObject *)(p->second);
                    ptrObj->heading += ptrUser->turning * dt;
                    ptrObj->xvel += sin(ptrObj->heading) * ptrUser->a * dt;
                    ptrObj->yvel += cos(ptrObj->heading) * ptrUser->a * dt;
                    ptrObj->x += ptrObj->xvel * dt;
                    ptrObj->y += ptrObj->yvel * dt;
                    ptrObj->color = ptrUser->color;
                    if (ptrObj->x > 390.0) ptrObj->x -= 780.0;          // bounds checking
                    if (ptrObj->x < -390.0) ptrObj->x += 780.0;
                    if (ptrObj->y > 290.0) ptrObj->y -= 580.0;
                    if (ptrObj->y < -290.0) ptrObj->y += 580.0;
                }

                p++;
            }

            SDL_Delay(20);
        }


    } else {                  // *************************************** CLIENT CODE
        map<int,NetUser>::iterator p;
        map<int,NetObject *>::iterator po;
        string ip;  // where to connect
        if (argc == 2) ip = argv[1]; else ip = "127.0.0.1";

        netdata = new NetData(NetData::CLIENT);
        netdata->connect(ip, 5074);
        netdata->me.nick = "Test";
        netdata->me.changed = true; // Mark all info for transmission

        if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )    // initialize SDL video
        {
            printf( "Unable to init SDL: %s\n", SDL_GetError() );
            return 1;
        }
        atexit(SDL_Quit);// make sure SDL cleans up before exit

        // create a new window
        screen = SDL_SetVideoMode(800, 600, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);
        if ( !screen )
        {
            printf("Unable to set 800x600 video: %s\n", SDL_GetError());
            return 1;
        }

        dstrect.x = (screen->w)/2; dstrect.y = (screen->h)/2;
        dstrect.w = 10; dstrect.h = 10;

        // program main loop
        bool done = false;
        while (!done)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                // check for messages
                switch (event.type)
                {
                 case SDL_QUIT:
                    done = true;
                    break;
                 case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                     case SDLK_ESCAPE:
                     case SDLK_q:
                        done = true;
                        break;
                     case SDLK_c:
                        netdata->me.color = 63+ 64*(rand()%4) + 256*64*(rand()%4) + 65536*64*(rand()%4);
                        netdata->me.changed = true;
                        break;
                     case SDLK_w:
                        netdata->me.a = 300; netdata->me.changed = true;
                        break;
                     case SDLK_a:
                        netdata->me.turning -= 5.5; netdata->me.changed = true;
                        break;
                     case SDLK_d:
                        netdata->me.turning += 5.5; netdata->me.changed = true;
                        break;
                     default:
                        break;
                    } // inner switch

                    break;
                 case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                     case SDLK_w:
                        netdata->me.a = 0; netdata->me.changed = true;
                        break;
                     case SDLK_a:
                        netdata->me.turning += 5.5; netdata->me.changed = true;
                        break;
                     case SDLK_d:
                        netdata->me.turning -= 5.5; netdata->me.changed = true;
                        break;
                     default:
                        break;
                    }
                }
            } // end of message processing

            SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0)); // clear screen

            po = netdata->netobjects.begin();
            while (po != netdata->netobjects.end()) {
                if (typeid(*po->second) == typeid(NetGameObject))
                {
                    NetGameObject *ptrObj = (NetGameObject *)(po->second);
                    dstrect.w = dstrect.h = 10;
                    dstrect.x = (screen->w)/2 + ptrObj->x - 5;
                    dstrect.y = (screen->h)/2 - ptrObj->y + 5;
                    SDL_FillRect(screen, &dstrect, ptrObj->color);
                    dstrect.w = dstrect.h = 6;
                    dstrect.x += sin(ptrObj->heading) * 8. + 3;
                    dstrect.y -= cos(ptrObj->heading) * 8. - 3;
                    SDL_FillRect(screen, &dstrect, ptrObj->color);

                    cout << "("<<ptrObj->x<<","<<ptrObj->y<<"), color "<<ptrObj->color<<", heading "<<ptrObj->heading<<endl;
                }
                po++;
            }

            SDL_Flip(screen);        // finally, update the screen :)

            netdata->service();
            SDL_Delay(5);
        } // end main loop


        // all is well ;)
        delete netdata;
        printf("Exited cleanly\n");
        return 0;
    }
}
