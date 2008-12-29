#ifndef AP_FRAMELISTENER_H
#define AP_FRAMELISTENER_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>

using namespace Ogre;

class GameEngine;


class ApFrameListener : public FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
 public:

    ApFrameListener(RenderWindow *window, GameEngine *engine, CEGUI::System *guiSystem);
    virtual ~ApFrameListener();

    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);
    bool frameStarted(const FrameEvent &evt);

    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseMoved(const OIS::MouseEvent &arg);

    // CEGUIn testausta varten luotu tilapäisfunktio.
    bool quit(const CEGUI::EventArgs&);

 protected:

    OIS::Keyboard *mKeyboard_;
    OIS::InputManager *mInputManager_;

 private:

    GameEngine *engine_;
    RenderWindow *window_;
    CEGUI::System *guiSystem_;

    void _initializeOIS(RenderWindow *window);
    CEGUI::MouseButton _convertButton(OIS::MouseButtonID buttonID);

};

#endif
