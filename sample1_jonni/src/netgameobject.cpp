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


NetGameObject::NetGameObject(int _id, int _uid)
{
    id = _id;   uid = _uid;
    x = y = z = xvel = yvel = zvel = heading = 0.0;
    color = 0x00FFFFFF;
    changed = false;
}

int NetGameObject::serialize(enet_uint8 buffer[], int start, int buflength)
{
    int length = 0;

    *(int *)(buffer+start+length) = id;                     length += 4;
    *(enet_uint8 *)(buffer+start+length) = NetData::OBJECT_TYPE_NETGAMEOBJECT; length++;
    *(int *)(buffer+start+length) = uid;                    length += 4;
//    *(unsigned int *)(buffer+start+length) = age;           length += 4;
    *(float *)(buffer+start+length) = x;                    length += 4;
    *(float *)(buffer+start+length) = y;                    length += 4;
    *(float *)(buffer+start+length) = z;                    length += 4;
    *(float *)(buffer+start+length) = xvel;                 length += 4;
    *(float *)(buffer+start+length) = yvel;                 length += 4;
    *(float *)(buffer+start+length) = zvel;                 length += 4;
    *(float *)(buffer+start+length) = heading;              length += 4;
    *(float *)(buffer+start+length) = a;                    length += 4;
    *(float *)(buffer+start+length) = turning;              length += 4;
    *(int *)(buffer+start+length) = color;                  length += 4;

    return length;
}

int NetGameObject::unserialize(enet_uint8 buffer[], int start)
{
    int length = 0;
    enet_uint8 objtype;

    if (id == *(int *)(buffer+start)) {                     length += 4;
        objtype = *(enet_uint8 *)(buffer+start+length);     length++;
        uid = *(int *)(buffer+start+length);                length += 4;
//        age = *(unsigned int*)(buffer+start+length);        length += 4;
        x = *(float *)(buffer+start+length);                length += 4;
        y = *(float *)(buffer+start+length);                length += 4;
        z = *(float *)(buffer+start+length);                length += 4;
        xvel = *(float *)(buffer+start+length);             length += 4;
        yvel = *(float *)(buffer+start+length);             length += 4;
        zvel = *(float *)(buffer+start+length);             length += 4;
        heading = *(float *)(buffer+start+length);          length += 4;
        a = *(float *)(buffer+start+length);                length += 4;
        turning = *(float *)(buffer+start+length);          length += 4;
        color = *(int *)(buffer+start+length);              length += 4;
    } else cout << "FOULED!" << endl;

    return length;
}




void NetGameObject::advance(float dt)
{
    heading += turning * dt;
    xvel += sin(heading) * a * dt;
    yvel += cos(heading) * a * dt;
    x += xvel * dt;
    y += yvel * dt;
}
