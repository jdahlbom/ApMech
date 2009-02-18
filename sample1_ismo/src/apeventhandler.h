#ifndef AP_EVENTHANDLER_H
#define AP_EVENTHANDLER_H

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
    void move(float msTimeSinceLast);

 private:
    GameEngine *mEngine;
    ApGui *mApGui;
    void markAsChanged();

    float mControlForward;
    float mControlLeft;

    float mMovementSpeed;
    float mTurningSpeed;

};


#endif
