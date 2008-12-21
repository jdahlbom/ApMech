#include <OIS/OIS.h>

#include "ExampleApplication.h"
#include "ExampleFrameListener.h"

class TutorialFrameListener : public ExampleFrameListener, public OIS::MouseListener, public OIS::KeyListener
{
public:
    TutorialFrameListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr);
    virtual ~TutorialFrameListener();

    // Overriding the default processUnbufferedKeyInput so the key updates we define
    // later on work as intended.
    bool processUnbufferedKeyInput(const FrameEvent& evt);

    // Overriding the default processUnbufferedMouseInput so the Mouse updates we define
    // later on work as intended.
    bool processUnbufferedMouseInput(const FrameEvent& evt);

    bool frameStarted(const FrameEvent &evt);

    // Overridden OIS::MouseListener functions.
    bool mouseMoved(const OIS::MouseEvent &arg);
    bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

    // Overridden OIS::KeyListener functions
    bool keyPressed( const OIS::KeyEvent &arg );
    bool keyReleased( const OIS::KeyEvent &arg );

protected:
    bool mMouseDown;       // Whether or not the left mouse button was down last frame
    Real mToggle;          // The time left until next toggle
    Real mRotate;          // The rotate constant
    Real mMove;            // The movement constant
    SceneManager *mSceneMgr;   // The current SceneManager
    SceneNode *mCamNode;   // The SceneNode the camera is currently attached to

private:
    bool mContinue;         //Whether or not the main loop should keep looping.
};

