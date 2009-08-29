#include "functions.h"
#define PI 3.141592653589793238462643383279

string uint2ipv4(unsigned int ip)
{
    stringstream out;
    out << (ip&255) <<"."<< ((ip>>8)&255) <<"."<< ((ip>>16)&255) <<"."<< (ip>>24);
    return out.str();
}


void hexprint(enet_uint8 *buffer, int length)
{
    enet_uint8 u;
    cout << "Buf length = "<<length<<": ";
    cout << "(";
    for (int i=0; i<length; i++) {
        u = buffer[i];

        if ((u > 64) && (u < 91)) cout << char(u);
        else if ((u > 96) && (u < 123)) cout << char(u);
        else cout << int(u);

        if (i != length-1) cout << ",";
    }

    cout << ")"<<endl;
}

template <class T> inline std::string to_string (const T &t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

long int getTicks()
{
    static long int startticks = -1;
    static time_t starttime_t;
    static timeval now;

    gettimeofday(&now, NULL);

    if (startticks != -1) { // then the function is initialized already
        return (now.tv_sec - starttime_t)*1000 + (now.tv_usec - startticks)/1000;
    } else {
        starttime_t = now.tv_sec;
        startticks = now.tv_usec/1000;
        return 0;
    }
}

void putpixel(SDL_Surface *s, int x, int y, int r, int g, int b)
{
	int *scr = (int *)(s->pixels);
	if ((x >= 0) && (x < s->w) && (y >= 0) && (y < s->h))
		scr[x + y*s->w] = SDL_MapRGB(s->format, r, g, b); // Red, Green, Blue
}

void putline2(SDL_Surface *s, int _x, int _y, int _x2, int _y2, int r, int g, int b)
{
	float vali, x = _x, y = _y, x2 = _x2, y2 = _y2;

	float k;

    if ((_x == _x2) && (_y == _y2)) {
        putpixel(s, _x, _y, r, g, b);
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
			putpixel (s, round(x), round(y), r, g, b);
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
			putpixel (s, round(x), round(y), r, g, b);
			x=x+k;
		}
	}
}

void puttriangle(SDL_Surface *s, int x, int y, int x2, int y2, int x3, int y3, int r, int g, int b)
{
	putline2 (s, x, y, x2, y2, r, g, b);
	putline2 (s, x2, y2, x3, y3, r, g, b);
	putline2 (s, x3, y3, x, y, r, g, b);
}

void puttriangle_c(SDL_Surface *s, int x, int y, int x2, int y2, int x3, int y3, int r, int g, int b)
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
		putline2(s, round(xa), round(y_), round(xb), round(y_), r, g, b);
		xa=xa+k1;
		xb=xb+k2;
	}
	xa = x3_;
	xb = x3_;

	for (y3_=y3_; y3_>y2_; y3_=y3_-1)
	{
		putline2(s, round(xa), round(y3_), round(xb), round(y3_), r, g, b);
		xa=xa-k4;
		xb=xb-k3;
	}

}

void putsputnik(SDL_Surface *s, int x, int y, int size, float angle, int r, int g, int b)
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

	puttriangle_c (s, kulma1x, kulma1y, kulma2x, kulma2y, kulma3x, kulma3y, r, g, b);
}

int drawObject(SDL_Surface *s, float x, float y, ap::net::NetObject *no)
{
    NetGameObject *ngo = dynamic_cast<NetGameObject *>(no);
    Projectile *po = dynamic_cast<Projectile *>(no);
    StarField *so = dynamic_cast<StarField *>(no);

    if (ngo) {  // If it's a NetGameObject, draw a sputnik
        int sx, sy;
        sx = fmod(ngo->loc.x + 6000.0 -x + s->w/2.0, 4000.0) - 2000.0;
        sy = fmod(-ngo->loc.y + 6000.0 +y + s->h/2.0, 4000.0) - 2000.0;

    //    putsputnik(s, (s->w)/2 + loc.x - x, (s->h)/2 - loc.y + y,
        putsputnik(s, sx, sy,
            15.0, ngo->loc.heading - PI/2, (ngo->color >> 16), (ngo->color >> 8)&255, ngo->color&255);
        return 1;
    }
    if (po) {
        int sx, sy;
        sx = fmod(po->loc.x + 6000.0 -x + s->w/2.0, 4000.0) - 2000.0;
        sy = fmod(-po->loc.y + 6000.0 +y + s->h/2.0, 4000.0) - 2000.0;

        SDL_Rect dstrect;
        dstrect.w = dstrect.h = 3;
        dstrect.x = sx; //(s->w)/2 + loc.x - 1 - x;
        dstrect.y = sy; //(s->h)/2 - loc.y + 1 + y;
        SDL_FillRect(s, &dstrect, 0x00FFAA00);
        return 1;
    }
    if (so) {
        const int stars = 4000;
        int *scr = (int *)(s->pixels);

        int seed = 452525936;
        int starx, stary, brightness;

        for (int i=0; i<stars; i++)
        {
            starx = fmod((seed >> 16) % 4000 + 2000.0 - x + s->w/2, 4000.0) - 2000.0;
            seed = ((214013*seed) + 2531011) & 0x7FFFFFFF;      // Step random func
            stary = fmod((seed >> 16) % 4000 + 2000.0 + y + s->h/2, 4000.0) - 2000.0;
            seed = ((214013*seed) + 2531011) & 0x7FFFFFFF;      // Step random func
            brightness = (seed >> 16) % 256;
            seed = ((214013*seed) + 2531011) & 0x7FFFFFFF;      // Step random func

            if ((starx >= 0) && (stary >= 0) && (starx < s->w) && (stary < s->h))
                scr[starx + stary*s->w] = brightness + brightness*256 + brightness*65536;
        }

        return 1;
    }
    return 0;
}

int drawTextArea(SDL_Surface *s, float x, float y, const std::string &content, TTF_Font *font)
{
    SDL_Color color = {255,255,255};
    SDL_Surface *tsurf;
    SDL_Rect dstrect;
    dstrect.w = dstrect.h = 3; dstrect.x = x; dstrect.y = y;

    istringstream iss(content);
    string line;
    while (getline(iss, line)) {
        tsurf = TTF_RenderText_Blended(font, line.c_str(), color);
        SDL_BlitSurface(tsurf, NULL, s, &dstrect);
        SDL_FreeSurface(tsurf);
        dstrect.y += TTF_FontLineSkip(font);
    }
    return 0;
}
