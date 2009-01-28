#ifndef AP_FRAMELISTENER_H
#define AP_FRAMELISTENER_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>

using namespace Ogre;

class ApEventHandler;


class ApFrameListener : public FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
 public:

    ApFrameListener(RenderWindow *window, ApEventHandler *handler);
    virtual ~ApFrameListener();

    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);
    bool frameStarted(const FrameEvent &evt);

    bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
    bool mouseMoved(const OIS::MouseEvent &arg);

 protected:

    OIS::Keyboard *mKeyboard;
    OIS::Mouse *mMouse;
    OIS::InputManager *mInputManager;

 private:

    ApEventHandler *mHandler;
    RenderWindow *mWindow;
    CEGUI::System *mGuiSystem;

    void initializeOIS(RenderWindow *window);
    CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID);

};

#endif
