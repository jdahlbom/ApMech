#ifndef AP_EVENTHANDLER_H
#define AP_EVENTHANDLER_H

#include <CEGUI/CEGUI.h>
#include "gameengine.h"
#include "apgui.h"

class ApEventHandler
{
 public:
    ApEventHandler(GameEngine *engine);
    virtual ~ApEventHandler();

    void setGui(ApGui *guiPointer);

    // Toiminnallisuutta, jota framelistener kutsuu
    // TODO: Miksi näillä on bool-palautusarvo?
    bool quit();
    bool moveUp();
    bool moveDown();
    bool moveLeft();
    bool moveRight();
    void toggleMainMenu();

 protected:
    void move(float forward, float right);

 private:
    GameEngine *mEngine;
    ApGui *mApGui;
    void markAsChanged();

};


#endif
