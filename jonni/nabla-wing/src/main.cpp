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
#include "SDL_ttf.h"

#include <iostream>
#include <cstdlib>  // for random
#include <cmath>    // for sin,cos in the server

#include "net/netdata.h"
#include "netgameobject.h"
#include "starfield.h"
#include "functions.h"
#include "constants.h"
#include "NablaControl.h"

using namespace std;
using namespace ap::net;

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
        string ip;
        ap::NablaControl control;   // For controlling a ship!
        TTF_Font *font;

        if (TTF_Init() == -1) cout <<"Error initializing SDL_ttf"<<endl;
        font = TTF_OpenFont("forgotten.ttf", 22);
        if (!font) {
            cout << "Error loading font: "<<TTF_GetError()<<endl;
            return 0;
        }

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
        netdata->me.nick = "Anonymous";
        netdata->me.changed = true;         // Mark this info for transmission
        netdata->me.setControlPtr(&control);

        bool done = false, writemode = false;
        ap::net::NetEvent netevent;
        string writeinput;
        std::list<string> textBox(5,"");
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
                    if (writemode) switch (event.key.keysym.sym) {
                     case SDLK_RETURN:
                     {
                        cout << writeinput << endl;
                        if (writeinput[0] == '/') { // That is, it's a command
                            if (writeinput.compare(0, 6, "/nick ") == 0) {
                                netdata->me.nick = writeinput.substr(6);
                                netdata->me.changed = true;
                            } else {
                                textBox.pop_front(); textBox.push_back("error: unknown command");
                            }
                        } else {
                            NetMessage netmsg(writeinput, netdata->me.uid);
                            netdata->sendMessage(netmsg);
                        }
                        writeinput = "";
                        writemode = false;
                        SDL_EnableUNICODE(0);       // disable
                        SDL_EnableKeyRepeat(0,0);   // disable
                        break;
                     }
                     case SDLK_BACKSPACE:
                        if (writeinput.size() > 0) writeinput.resize(writeinput.size() - 1);
                        break;
                     case SDLK_LALT:
                     case SDLK_LCTRL:
                     case SDLK_LSHIFT:
                     case SDLK_LMETA:
                     case SDLK_LSUPER:
                     case SDLK_RALT:
                     case SDLK_RCTRL:
                     case SDLK_RSHIFT:
                     case SDLK_RMETA:
                     case SDLK_RSUPER:
                     case SDLK_NUMLOCK:
                     case SDLK_CAPSLOCK:
                     case SDLK_SCROLLOCK:
                     case SDLK_MODE:
                     case SDLK_MENU:
                        break;    // Nevermind modifiers!
                     default:
                        writeinput.append(1, event.key.keysym.unicode);
                        break;
                    } else switch (event.key.keysym.sym)
                    {
                     case SDLK_t:
                        writeinput = "";
                        writemode = true;
                        SDL_EnableUNICODE(1);
                        SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
                        break;
                     case SDLK_ESCAPE:
                     case SDLK_q:
                        done = true;
                        break;
                     case SDLK_c:
                        control.color = 63+64*(rand()%4) + 256*64*(rand()%4) + 65536*64*(rand()%4);
                        netdata->me.changed = true;
                        break;
                     case SDLK_w:
                     case SDLK_UP:
                        control.a = 300; netdata->me.changed = true;
                        break;
                     case SDLK_a:
                     case SDLK_LEFT:
                        control.turning -= 5.5; netdata->me.changed = true;
                        break;
                     case SDLK_d:
                     case SDLK_RIGHT:
                        control.turning += 5.5; netdata->me.changed = true;
                        break;
                     case SDLK_SPACE:
                     case SDLK_PERIOD:
                        control.controls |= NetUser::SHOOT_MG; netdata->me.changed = true;
                        break;
                     default:
                        break;
                    }   // inner KEYDOWN switch

                    break;
                 case SDL_KEYUP:
                    if (!writemode) switch (event.key.keysym.sym)
                    {
                     case SDLK_w:
                     case SDLK_UP:
                        control.a = 0; netdata->me.changed = true;
                        break;
                     case SDLK_a:
                     case SDLK_LEFT:
                        control.turning += 5.5; netdata->me.changed = true;
                        break;
                     case SDLK_d:
                     case SDLK_RIGHT:
                        control.turning -= 5.5; netdata->me.changed = true;
                        break;
                     case SDLK_SPACE:
                     case SDLK_PERIOD:
                        control.controls &= ~NetUser::SHOOT_MG; netdata->me.changed = true;
                        break;
                     default:
                        break;
                    }   // inner KEYUP switch
                }
            } // end of SDL message processing

            while (netdata->pollEvent(netevent)) {
                switch (netevent.type)
                {
                 case NetData::EVENT_DELETEOBJECT:
                    netdata->removeObject(netevent.uid);
                    break;
                 case NetData::EVENT_MESSAGE:
                    textBox.pop_front(); textBox.push_back(netevent.message->data.c_str());
                    break;
                }
            }

            oldticks = newticks; newticks = getTicks(); dt = float(newticks - oldticks) * 0.001;

            // Now, advance all objects. That means, let them predict where they will be after dt.
            while (NetObject *nop = netdata->eachObject())
                if (nop->advance(dt) == -1) netdata->removeObject(nop->id);

            // Advance our avatar according to our input state!
            // NOTICE that this is merely for more comfortable controls. Server is king, and overwrites when he wants.
            NetGameObject *myobj = netdata->getAvatarObject<NetGameObject *>();
            if (myobj) myobj->control(netdata->me);

            netdata->sendChanges();
            netdata->receiveChanges(); // IF we get an update from server, that overwrites previous advances

            SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0)); // clear screen

            if (myobj) { xoffset = myobj->loc.x; yoffset = myobj->loc.y; }

            while (NetObject *nop = netdata->eachObject())
                drawObject(screen, xoffset, yoffset, nop);

            // Draw the radar box
            while (NetGameObject *radarobj = netdata->eachObject<NetGameObject*>(ap::OBJECT_TYPE_NETGAMEOBJECT))
            {
                SDL_Rect dstrect;
                dstrect.w = dstrect.h = 3;
                dstrect.x = 750 + radarobj->loc.x / 50;
                dstrect.y = 550 - radarobj->loc.y / 50;
                SDL_FillRect(screen, &dstrect, radarobj->color);
            }

            drawTextArea(screen, 50, HEIGHT-TTF_FontLineSkip(font), writeinput.c_str(), font);
            drawTextArea(screen, 50, HEIGHT-TTF_FontLineSkip(font)*6, textBox, font);

            SDL_Flip(screen);       // finally, update the screen :)
            SDL_Delay(1);           // and delay. Could be removed, most probably. Maybe this allows some multiprocessing on single core systems though!
        } // end of MAIN LOOP


        delete netdata;           // all is well ;)
        printf("Exited cleanly\n");
        TTF_CloseFont(font); TTF_Quit();
        return 0;
    }
}
