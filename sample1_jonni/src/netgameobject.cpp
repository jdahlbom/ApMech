#include "netgameobject.h"

/**//* This mystical part here pushes exactly ONE object of this type to the prototype map, */
/**//* in order to easily unserialize stuff. Idea gotten from C++ FAQ Lite by Marshall Cline*/
/**/class NetGameObjectInject {
/**/    public: NetGameObjectInject() {
/**/        netobjectprototypes().insert(make_pair(NetData::OBJECT_TYPE_NETGAMEOBJECT, new NetGameObject(0)));
/**/    }
/**/};
/**/static NetGameObjectInject __netgameobjectinject;
/**//* A create method like this is also ABSOLUTELY REQUIRED in descendants of NetObject. */
/**/NetObject *NetGameObject::create(int id)
/**/{
/**/    NetObject *ptr = new NetGameObject(id);
/**/    return ptr;
/**/}

#define PI 3.141592653589793238462643383279
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







NetGameObject::NetGameObject(int _id, int _uid)
{
    id = _id;   uid = _uid;
//    x = y = z = xvel = yvel = zvel = heading = 0.0;
    color = 0x00FFFFFF;
    changed = false;
    mg_delay = 0;
    loc.radius = 11.0; loc.x = loc.y = 50;
}

int NetGameObject::serialize(enet_uint8 buffer[], int start, int buflength)
{
    int length = 0;

    *(int *)(buffer+start+length) = id;                     length += 4;
    *(enet_uint8 *)(buffer+start+length) = NetData::OBJECT_TYPE_NETGAMEOBJECT; length++;
    *(int *)(buffer+start+length) = uid;                    length += 4;
    *(int *)(buffer+start+length) = color;                  length += 4;
    length += loc.serialize(buffer, start+length, buflength);

    return length;
}

int NetGameObject::unserialize(enet_uint8 buffer[], int start)
{
    int length = 0;
    enet_uint8 objtype;

    if (id == *(int *)(buffer+start+length)) {              length += 4;
        objtype = *(enet_uint8 *)(buffer+start+length);     length++;
        uid = *(int *)(buffer+start+length);                length += 4;
        color = *(int *)(buffer+start+length);              length += 4;
        length += loc.unserialize(buffer, start+length);
    } else cout << "FOULED in NetGameObject::unserialize!" << endl;

    return length;
}




int NetGameObject::advance(float dt)
{
    loc.advance(dt);
    mg_delay -= dt; if (mg_delay < 0) mg_delay = 0;
    return 0;
}

int NetGameObject::draw(SDL_Surface *s, float x, float y)
{
    int sx, sy;
    sx = fmod(loc.x + 6000.0 -x + s->w/2.0, 4000.0) - 2000.0;
    sy = fmod(-loc.y + 6000.0 +y + s->h/2.0, 4000.0) - 2000.0;

//    putsputnik(s, (s->w)/2 + loc.x - x, (s->h)/2 - loc.y + y,
    putsputnik(s, sx, sy,
        15.0, loc.heading - PI/2, (color >> 16), (color >> 8)&255, color&255);
    return 1;
}

list<NetObject *> *NetGameObject::control(NetUser &user, bool createobjects)
{
    loc.a = user.a;
    loc.turning = user.turning;
    color = user.color;
    if (createobjects)
    {
        if ((user.controls & NetUser::SHOOT_MG) && (mg_delay == 0)) {
            list<NetObject *> *result = new list<NetObject *>();

            Projectile *pProj;
            mg_delay = 0.2;
            pProj = new Projectile(-1);
            pProj->loc = loc;
            pProj->loc.a = 0;
            pProj->loc.x += sin(pProj->loc.heading) * 16.0;
            pProj->loc.y += cos(pProj->loc.heading) * 16.0;
            pProj->loc.xvel += sin(pProj->loc.heading) * 400.0;
            pProj->loc.yvel += cos(pProj->loc.heading) * 400.0;
            result->push_back(dynamic_cast<NetObject *>(pProj));

            return result;
        }
    }
    return NULL;
}
