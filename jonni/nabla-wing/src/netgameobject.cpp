#include "netgameobject.h"

/**//* This mystical part here pushes exactly ONE object of this type to the prototype map, */
/**//* in order to easily unserialize stuff. Idea gotten from C++ FAQ Lite by Marshall Cline*/
/**/class NetGameObjectInject {
/**/    public: NetGameObjectInject() {
/**/        netobjectprototypes().insert(make_pair(ap::OBJECT_TYPE_NETGAMEOBJECT, new NetGameObject(0)));
/**/    }
/**/};
/**/static NetGameObjectInject __netgameobjectinject;
/**//* A create method like this is also ABSOLUTELY REQUIRED in descendants of NetObject. */
/**/NetObject *NetGameObject::create(ap::uint32 id) const
/**/{
/**/    NetObject *ptr = new NetGameObject(id);
/**/    return ptr;
/**/}


NetGameObject::NetGameObject(int _id, int _uid)
{
    id = _id;   uid = _uid;
//    x = y = z = xvel = yvel = zvel = heading = 0.0;
    controls = NULL;
    color = 0x00FFFFFF;
    changed = false;
    mg_delay = 0;
    loc.radius = 11.0; loc.x = loc.y = 50;
}

uint8 NetGameObject::getObjectType() const
{
    return ap::OBJECT_TYPE_NETGAMEOBJECT;
}


int NetGameObject::serialize(ap::uint8 buffer[], int start, int buflength) const
{
    int length = 0;

    *(int *)(buffer+start+length) = id;                     length += 4;
    *(enet_uint8 *)(buffer+start+length) = ap::OBJECT_TYPE_NETGAMEOBJECT; length++;
    *(int *)(buffer+start+length) = uid;                    length += 4;
    *(int *)(buffer+start+length) = color;                  length += 4;
    length += loc.serialize(buffer, start+length, buflength);

    return length;
}

int NetGameObject::unserialize(ap::uint8 buffer[], int start)
{
    int length = 0;
    enet_uint8 objtype;

    if (id == *(uint32 *)(buffer+start+length)) {              length += 4;
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

list<NetObject *> *NetGameObject::control(NetUser &user, bool createobjects)
{
    // TODO: DO SOMETHING!
    ap::NablaControl *control;
    control = dynamic_cast<ap::NablaControl *>(user.controls);
    if (!control) cout << "netgameobject.cpp: Control is of wrong type!\n";

    loc.a = control->a;
    loc.turning = control->turning;
    color = control->color;
    if (createobjects)
    {
        if ((control->controls & NetUser::SHOOT_MG) && (mg_delay == 0)) {
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
