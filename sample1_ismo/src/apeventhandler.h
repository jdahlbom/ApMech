#ifndef AP_EVENTHANDLER_H
#define AP_EVENTHANDLER_H

#include <CEGUI/CEGUI.h>

#include "gameengine.h"

class ApEventHandler
{
 public:
    ApEventHandler(GameEngine *engine);
    virtual ~ApEventHandler();

    bool quit(const CEGUI::EventArgs &arg);

 protected:

 private:
    GameEngine *engine_;

};


#endif
