#include "location.h"


Location::Location()
{
    x = y = z = xvel = yvel = zvel = heading = a = turning = 0.0;
}

int Location::serialize(enet_uint8 buffer[], int start, int buflength)
{
    int length = 0;

    *(float *)(buffer+start+length) = x;                    length += 4;
    *(float *)(buffer+start+length) = y;                    length += 4;
    *(float *)(buffer+start+length) = z;                    length += 4;
    *(float *)(buffer+start+length) = xvel;                 length += 4;
    *(float *)(buffer+start+length) = yvel;                 length += 4;
    *(float *)(buffer+start+length) = zvel;                 length += 4;
    *(float *)(buffer+start+length) = heading;              length += 4;
    *(float *)(buffer+start+length) = a;                    length += 4;
    *(float *)(buffer+start+length) = turning;              length += 4;

    return length;
}

int Location::unserialize(enet_uint8 buffer[], int start)
{
    int length = 0;

    x = *(float *)(buffer+start+length);                length += 4;
    y = *(float *)(buffer+start+length);                length += 4;
    z = *(float *)(buffer+start+length);                length += 4;
    xvel = *(float *)(buffer+start+length);             length += 4;
    yvel = *(float *)(buffer+start+length);             length += 4;
    zvel = *(float *)(buffer+start+length);             length += 4;
    heading = *(float *)(buffer+start+length);          length += 4;
    a = *(float *)(buffer+start+length);                length += 4;
    turning = *(float *)(buffer+start+length);          length += 4;

    return length;
}

void Location::advance(float dt)
{
    float r = 0.0001+sqrt((x-10)*(x-10) + y*y), rx, ry, fx, fy; // rx,ry is the unit vector to r direction
    rx = -(x-10); ry = -y;
    fx = rx/(r*r*r); fy = ry/(r*r*r);

    heading += turning * dt;
    xvel += sin(heading) * a * dt;
    yvel += cos(heading) * a * dt;
    x += xvel * dt;
    y += yvel * dt;

    if (x > 400.0) x -= 800.0;          // bounds checking
    else if (x < -400.0) x += 800.0;
    if (y > 300.0) y -= 600.0;
    else if (y < -300.0) y += 600.0;
}

bool Location::collision(Location &b)
{
    if ((x-b.x)*(x-b.x) + (y-b.y)*(y-b.y) <= (radius+b.radius)*(radius+b.radius)) return true;
    return false;
}


Location Location::operator=(Location proto)
{
    x = proto.x;            y = proto.y;        z = proto.z;
    xvel = proto.xvel;      yvel = proto.yvel;  zvel = proto.zvel;
    heading = proto.heading;a = proto.a;        turning = proto.turning;
    return *this;
}

std::ostream &operator<<(std::ostream &stream, Location &l)
{
    stream << "Location: [r=("<<l.x<<","<<l.y<<","<<l.z<<") v=("<<l.xvel<<","<<l.yvel<<","<<l.zvel<<") ";
    stream << "heading="<<(l.heading*180./3.141592)<<", a="<<l.a<<"]";
    return stream;
}
