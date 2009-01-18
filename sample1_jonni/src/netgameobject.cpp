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
//    x = y = z = xvel = yvel = zvel = heading = 0.0;
    color = 0x00FFFFFF;
    changed = false;
    mg_delay = 0;
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

    if (id == *(int *)(buffer+start)) {                     length += 4;
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

vector<NetObject *> *NetGameObject::control(NetUser &user)
{
    loc.a = user.a;
    loc.turning = user.turning;
    color = user.color;
    if ((user.controls & NetUser::SHOOT_MG) && (mg_delay == 0)) {
        vector<NetObject *> *result = new vector<NetObject *>();

        Projectile *pProj;
        mg_delay = 0.2;
        pProj = new Projectile(-1);
        pProj->loc = loc;
        pProj->loc.a = 0;
        pProj->loc.xvel += sin(pProj->loc.heading) * 400.0;
        pProj->loc.yvel += cos(pProj->loc.heading) * 400.0;
        result->push_back(dynamic_cast<NetObject *>(pProj));

        return result;
    }
    return NULL;
}
