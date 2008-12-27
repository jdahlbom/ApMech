#ifndef AP_FRAMELISTENER_H
#define AP_FRAMELISTENER_H

#include <Ogre.h>
#include <OIS/OIS.h>

using namespace Ogre;

class GameEngine;

class ApFrameListener : public FrameListener, public OIS::KeyListener
{
 private:

    GameEngine *engine_;
    RenderWindow *window_;

 protected:

    OIS::Keyboard *mKeyboard_;
    OIS::InputManager *mInputManager_;

 public:

    ApFrameListener(RenderWindow *window, GameEngine *engine);

    virtual ~ApFrameListener() {}

    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);
    bool frameStarted(const FrameEvent &evt);
};

#endif
