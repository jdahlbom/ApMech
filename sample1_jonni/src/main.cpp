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

    long int oldticks, newticks;    // These here for tracking time
    float dt;                       //

    srand(time(NULL));

    if ((argc == 2) && (strcmp(argv[1], "-server") == 0)) { // ********* SERVER CODE
        map<int,NetObject *>::iterator p;
        map<int, NetUser>::iterator iUser;

        cout << "Created a server."<<endl;
        netdata = new NetData(NetData::SERVER);

        cout << "Serving "<< netobjectprototypes().size()<<" objects."<<endl;

        newticks = getTicks();

        while (1) {             // Server main loop
            netdata->service();
            oldticks = newticks; newticks = getTicks(); dt = float(newticks - oldticks) * 0.001;

            p = netdata->netobjects.begin();
            while (p != netdata->netobjects.end()) {
                iUser = netdata->users.find(p->second->uid);

                if (typeid(*p->second) == typeid(NetGameObject))
                {
                    NetGameObject *ptrObj = (NetGameObject *)(p->second);

                    if (iUser != netdata->users.end()) // That means the object's owner is still logged in
                    {
                        ptrObj->a = iUser->second.a;
                        ptrObj->turning = iUser->second.turning;
                        ptrObj->color = iUser->second.color;
                    } else {                            // That means owner's disconnected, so paint it gray!
                        ptrObj->color = 0x00666666;
                    }
                    ptrObj->advance(dt);

                    if (ptrObj->x > 390.0) ptrObj->x -= 780.0;          // bounds checking
                    if (ptrObj->x < -390.0) ptrObj->x += 780.0;
                    if (ptrObj->y > 290.0) ptrObj->y -= 580.0;
                    if (ptrObj->y < -290.0) ptrObj->y += 580.0;
                }

                p++;
            }

            while (getTicks() < newticks + 40) SDL_Delay(1);    // take 40 ms per frame
//            SDL_Delay(40);                                    // alternative sleep, 40 + x ms
        }


    } else {                  // *************************************** CLIENT CODE
        map<int,NetObject *>::iterator po;
        string ip;
        if (argc == 2) ip = argv[1]; else ip = "127.0.0.1";         // server to connect to


        if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )  {                    // initialize SDL
            printf( "Unable to init SDL: %s\n", SDL_GetError() );   //
            return 1;                                               //
        }                                                           //
        atexit(SDL_Quit);                                           //
        screen = SDL_SetVideoMode(800, 600, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);
        if ( !screen ) {                                            //
            printf("Unable to set 800x600 video: %s\n", SDL_GetError());
            return 1;                                               //
        }                                                           // SDL initialized!


        netdata = new NetData(NetData::CLIENT);
        netdata->connect(ip, 5074);
        netdata->me.nick = "Test";
        netdata->me.changed = true;         // Mark this info for transmission


        bool done = false;
        while (!done)                       // MAIN LOOP **********************************************
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type)         // check for messages
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
                    }   // inner KEYDOWN switch

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
                    }   // inner KEYUP switch
                }
            } // end of SDL message processing


            oldticks = newticks; newticks = getTicks(); dt = float(newticks - oldticks) * 0.001;
            po = netdata->netobjects.begin();           // This part for linearly predicting where the objects
            while (po != netdata->netobjects.end()) {   // will be if they continued straight during one
                po->second->advance(dt);                // graphical frame.
                po++;                                   //
            }
            netdata->service();                         // IF we get an update from server, the previous work was in vain

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
                }
                po++;
            }

            SDL_Flip(screen);       // finally, update the screen :)
            SDL_Delay(1);           // and delay. Could be removed, most probably. Maybe this allows some multiprocessing on single core systems though!
        } // end of MAIN LOOP


        delete netdata;           // all is well ;)
        printf("Exited cleanly\n");
        return 0;
    }
}
