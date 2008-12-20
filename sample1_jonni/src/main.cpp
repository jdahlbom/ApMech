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
#include "netdata.h"
#include <cstdlib> // for random

using namespace std;

int main ( int argc, char** argv )
{
  SDL_Surface *screen;
  SDL_Rect dstrect;

  NetData *netdata;
  int client = 0;
  srand(time(NULL));

  if (argc == 2) {
    cout << "Created a server."<<endl;
    netdata = new NetData(NetData::SERVER);
  } else {
    netdata = new NetData(NetData::CLIENT); client = 1;
    netdata->connect("192.168.2.3", 5074);
    netdata->me.nick = "Test";
    netdata->me.x = -1; netdata->me.y = 2;
    netdata->me.color = 0x00FFFFFF;
    netdata->me.changed = true; // Mark all info for transmission

    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )    // initialize SDL video
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }
    atexit(SDL_Quit);// make sure SDL cleans up before exit

    // create a new window
    screen = SDL_SetVideoMode(640, 480, 24, SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }
    SDL_EnableKeyRepeat(10,10);

    dstrect.x = (screen->w)/2; dstrect.y = (screen->h)/2;
    dstrect.w = 10; dstrect.h = 10;
  }


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
                {
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
                        netdata->me.y--; netdata->me.changed = true;
                        break;
                     case SDLK_s:
                        netdata->me.y++; netdata->me.changed = true;
                        break;
                     case SDLK_a:
                        netdata->me.x--; netdata->me.changed = true;
                        break;
                     case SDLK_d:
                        netdata->me.x++; netdata->me.changed = true;
                        break;
                     default:
                        cout <<"P!";
                        break;
                    }

//                        SDL_MapRGB(screen->format, 200,100,0));
                    break;
                }
            } // end switch
        } // end of message processing
        if (client) {
            map<int,NetUser>::iterator p = netdata->users.begin();

            SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0)); // clear screen
            while (p != netdata->users.end()) {
                dstrect.x = (screen->w)/2 + p->second.x;
                dstrect.y = (screen->h)/2 + p->second.y;
                SDL_FillRect(screen, &dstrect, p->second.color);
                p++;
            }
            SDL_Flip(screen);        // finally, update the screen :)
           // cout << netdata->users.size() << " connections." << endl;
        }

        netdata->service();
        SDL_Delay(5);
    } // end main loop


  // all is well ;)
  delete netdata;
  printf("Exited cleanly\n");
  return 0;
}
