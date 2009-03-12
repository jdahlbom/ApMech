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
#include "starfield.h"
#include "functions.h"

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;

int servermain(int argc, char* argv[]);

int main ( int argc, char** argv )
{

    SDL_Surface *screen;
    NetData *netdata;
    float xoffset = 0, yoffset = 0;

    long int oldticks, newticks;    // These here for tracking time
    float dt;                       //

    srand(time(NULL));

    if ((argc == 2) && (strcmp(argv[1], "-server") == 0)) { // ********* SERVER CODE
        exit (servermain(argc, argv));
    } else {                  // *************************************** CLIENT CODE
        map<int,NetObject *>::iterator po;
        string ip;
        if (argc == 2) ip = argv[1]; else ip = "127.0.0.1";         // server to connect to


        if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )  {                    // initialize SDL
            printf( "Unable to init SDL: %s\n", SDL_GetError() );   //
            return 1;                                               //
        }                                                           //
        atexit(SDL_Quit);                                           //
        screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
        if ( !screen ) {                                            //
            printf("Unable to set 800x600 video: %s\n", SDL_GetError());
            return 1;                                               //
        }                                                           // SDL initialized!
        SDL_SetClipRect(screen, NULL);                              //

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
                        netdata->me.color = 63+64*(rand()%4) + 256*64*(rand()%4) + 65536*64*(rand()%4);
                        netdata->me.changed = true;
                        break;
                     case SDLK_w:
                     case SDLK_UP:
                        netdata->me.a = 300; netdata->me.changed = true;
                        break;
                     case SDLK_a:
                     case SDLK_LEFT:
                        netdata->me.turning -= 5.5; netdata->me.changed = true;
                        break;
                     case SDLK_d:
                     case SDLK_RIGHT:
                        netdata->me.turning += 5.5; netdata->me.changed = true;
                        break;
                     case SDLK_SPACE:
                     case SDLK_PERIOD:
                        netdata->me.controls |= NetUser::SHOOT_MG; netdata->me.changed = true;
                        break;
                     default:
                        break;
                    }   // inner KEYDOWN switch

                    break;
                 case SDL_KEYUP:
                    switch (event.key.keysym.sym)
                    {
                     case SDLK_w:
                     case SDLK_UP:
                        netdata->me.a = 0; netdata->me.changed = true;
                        break;
                     case SDLK_a:
                     case SDLK_LEFT:
                        netdata->me.turning += 5.5; netdata->me.changed = true;
                        break;
                     case SDLK_d:
                     case SDLK_RIGHT:
                        netdata->me.turning -= 5.5; netdata->me.changed = true;
                        break;
                     case SDLK_SPACE:
                     case SDLK_PERIOD:
                        netdata->me.controls &= ~NetUser::SHOOT_MG; netdata->me.changed = true;
                        break;
                     default:
                        break;
                    }   // inner KEYUP switch
                }
            } // end of SDL message processing

            oldticks = newticks; newticks = getTicks(); dt = float(newticks - oldticks) * 0.001;

            // Now, advance all objects. That means, let them predict where they will be after dt.
            for (po = netdata->netobjects.begin() ; po != netdata->netobjects.end() ; po++)
                if (po->second->advance(dt) == -1) {    // If somebody wants to be deleted,
                    delete po->second;                  // show no mercy!
                    netdata->netobjects.erase(po);
                }

            // Advance our avatar according to our input state!
            // NOTICE that this is merely for more comfortable controls. Server is king, and overwrites when he wants.
            NetGameObject *myobj = NULL;
            if ((netdata->myAvatarID > 0) && (netdata->netobjects.find(netdata->myAvatarID) != netdata->netobjects.end()))
                myobj = dynamic_cast<NetGameObject *>(netdata->netobjects.find(netdata->myAvatarID)->second);
            if (myobj) myobj->control(netdata->me);

            netdata->sendChanges();
            netdata->receiveChanges(); // IF we get an update from server, that overwrites previous advances

            SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0)); // clear screen

            if (myobj) { xoffset = myobj->loc.x; yoffset = myobj->loc.y; }

            for (po = netdata->netobjects.begin() ; po != netdata->netobjects.end() ; po++)
                po->second->draw(screen, xoffset, yoffset);
//            if (myobj) cout << "("<<floor(myobj->loc.x)<<","<<floor(myobj->loc.y)<<")"<<endl;
            // if (typeid(*po->second) == typeid(Projectile)) // if you need to discriminate



            for (po = netdata->netobjects.begin() ; po != netdata->netobjects.end() ; po++)
            {
                NetGameObject *radarobj = dynamic_cast<NetGameObject *>(po->second);
                if (radarobj != NULL)
                {
                    SDL_Rect dstrect;
                    dstrect.w = dstrect.h = 3;
                    dstrect.x = 750 + radarobj->loc.x / 50;
                    dstrect.y = 550 - radarobj->loc.y / 50;
                    SDL_FillRect(screen, &dstrect, radarobj->color);
                }
            }


            SDL_Flip(screen);       // finally, update the screen :)
            SDL_Delay(1);           // and delay. Could be removed, most probably. Maybe this allows some multiprocessing on single core systems though!
        } // end of MAIN LOOP


        delete netdata;           // all is well ;)
        printf("Exited cleanly\n");
        return 0;
    }
}
