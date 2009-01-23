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

SDL_Surface *screen;
#define PI 3.141592653589793238462643383279
const int WIDTH = 800;
const int HEIGHT = 600;

int servermain(int argc, char* argv[]);

void putpixel(int x, int y, int r, int g, int b)
{
	int *scr = (int *)(screen->pixels);
	if ((x >= 0) && (x < WIDTH) && (y >= 0) && (y < HEIGHT))
		scr[x + y*WIDTH] = SDL_MapRGB(screen->format, r, g, b); // Red, Green, Blue
}

void putline2(int _x, int _y, int _x2, int _y2, int r, int g, int b)
{
	float vali, x = _x, y = _y, x2 = _x2, y2 = _y2;

	float k;

    if ((_x == _x2) && (_y == _y2)) {
        putpixel(_x, _y, r, g, b);
        return;
    }

	if (abs (x2-x) > abs(y2-y))
	{
		if (x2<x)
		{
			vali = x;   x = x2;     x2 = vali;  // switch x
			vali = y;   y = y2;     y2 = vali;  // switch y
		}

        k = (y2-y)/(x2-x);

		for (x=x; x<=x2; x++)
		{
			putpixel (round(x), round(y), r, g, b);
			y=y+k;
		}
	} else 	{
		k = (x2-x)/(y2-y);

		if (y2<y)
		{
			vali = x;   x = x2;     x2 = vali;  // switch x
			vali = y;   y = y2;     y2 = vali;  // switch y
		}
		for (y=y; y<=y2; y++)
		{
			putpixel (round(x), round(y), r, g, b);
			x=x+k;
		}
	}
}

void puttriangle(int x, int y, int x2, int y2, int x3, int y3, int r, int g, int b)
{
	putline2 (x, y, x2, y2, r, g, b);
	putline2 (x2, y2, x3, y3, r, g, b);
	putline2 (x3, y3, x, y, r, g, b);
}

void puttriangle_c(int x, int y, int x2, int y2, int x3, int y3, int r, int g, int b)
{
	float x_, y_, x2_, y2_, x3_, y3_;
	if ((y <= y2) && (y <= y3))
	{
		y_ = y;
		x_ = x;

		if (y2 < y3)
		{
			y2_ = y2;
			x2_ = x2;
			y3_ = y3;
			x3_ = x3;
		}
		else
		{
			y2_ = y3;
			x2_ = x3;
			y3_ = y2;
			x3_ = x2;
		}
	}


	else if ((y2 < y) && (y2 <= y3))
	{
		y_ = y2;
		x_ = x2;

		if (y < y3)
		{
			y2_ = y;
			x2_ = x;
			y3_ = y3;
			x3_ = x3;
		}
		else
		{
            y2_ = y3;
			x2_ = x3;
			y3_ = y;
			x3_ = x;
		}
	}

	else if ((y3 < y) && (y3 < y2))
		{
		y_ = y3;
		x_ = x3;

		if (y2 < y)
		{
			y2_ = y2;
			x2_ = x2;
			y3_ = y;
			x3_ = x;
		}
		else
		{
			y2_ = y;
			x2_ = x;
			y3_ = y2;
			x3_ = x2;
		}
	}

	float k1, k2, k3, k4;
	if (y2_!=y_) k1 = (x2_-x_)/(y2_-y_);
	if (y3_!=y_)
	{
		k2 = (x3_-x_)/(y3_-y_);
		k4 = (x_-x3_)/(y_-y3_);
	}
	if (y2_!=y3_) k3 = (x2_-x3_)/(y2_-y3_);

	float xa, xb;
	xa = x_;
	xb = x_;

	for (y_=y_; y_<=y2_; y_++)
	{
		putline2(round(xa), round(y_), round(xb), round(y_), r, g, b);
		xa=xa+k1;
		xb=xb+k2;
	}
	xa = x3_;
	xb = x3_;

	for (y3_=y3_; y3_>y2_; y3_=y3_-1)
	{
		putline2(round(xa), round(y3_), round(xb), round(y3_), r, g, b);
		xa=xa-k4;
		xb=xb-k3;
	}

}

void putsputnik(int x, int y, int size, float angle, int r, int g, int b)
{
	float kulma1x, kulma1y, kulma2x, kulma2y, kulma3x, kulma3y, keskix, keskiy;
	kulma1x = float(x) + size * cos (angle);
	kulma1y = float(y) + size * sin (angle);
	keskix = float(x) - size/2 * cos (angle);
	keskiy = float(y) - size/2 * sin (angle);
	kulma2x = keskix + size/2 * cos (PI/2 + angle);
	kulma2y = keskiy + size/2 * sin (PI/2 + angle);
	kulma3x = keskix - size/2 * cos (PI/2 + angle);
	kulma3y = keskiy - size/2 * sin (PI/2 + angle);

	puttriangle_c (kulma1x, kulma1y, kulma2x, kulma2y, kulma3x, kulma3y, r, g, b);
}


int main ( int argc, char** argv )
{

//    SDL_Surface *screen;
    SDL_Rect dstrect;
    NetData *netdata;

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
            po = netdata->netobjects.begin();           // This part for linearly predicting where the objects
            while (po != netdata->netobjects.end()) {   // will be if they continued straight during one
                if (po->second->advance(dt) == -1) {    // graphical frame.
                    delete po->second;
                    netdata->netobjects.erase(po);
                }
                // If .. this is our ship, then advance it according to our input state.
                // NOTICE that whenever we get the ship status from server, server's version
                // overwrites our version, so this here is merely for the comfort of the controls.
                else if ((po->second->uid == netdata->me.uid) && (typeid(*po->second) == typeid(NetGameObject)))
                {
                    NetGameObject *myobj = dynamic_cast<NetGameObject *>(po->second);
                    if (myobj) myobj->control(netdata->me);
                }
                po++;                                   //
            }

            netdata->sendChanges();
            netdata->receiveChanges();                  // IF we get an update from server, the previous work was in vain

            SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0)); // clear screen

            po = netdata->netobjects.begin();
            while (po != netdata->netobjects.end()) {
                if (typeid(*po->second) == typeid(NetGameObject))
                {
                    NetGameObject *ptrObj = dynamic_cast<NetGameObject *>(po->second);
                    putsputnik((screen->w)/2 + ptrObj->loc.x, (screen->h)/2 - ptrObj->loc.y,
                        15.0, ptrObj->loc.heading - PI/2,
                        (ptrObj->color >> 16), (ptrObj->color >> 8)&255, ptrObj->color&255);
                }
                else if (typeid(*po->second) == typeid(Projectile))
                {
                    Projectile *ptrProj = dynamic_cast<Projectile *>(po->second);
                    dstrect.w = dstrect.h = 3;
                    dstrect.x = (screen->w)/2 + ptrProj->loc.x - 1;
                    dstrect.y = (screen->h)/2 - ptrProj->loc.y + 1;
                    SDL_FillRect(screen, &dstrect, 0x00FFAA00);
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
