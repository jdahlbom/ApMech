#ifndef __netevent_h__
#define __netevent_h__

#include <enet/enet.h>

class NetEvent
{
 public:
    enet_uint8 type;
    int uid;

    NetEvent();
    NetEvent(enet_uint8 _type, int a);

};

#endif
