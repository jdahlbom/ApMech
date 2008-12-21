#include "SampleFrameListener.h"

#include "CEGUI/CEGUI.h"
#include "OgreCEGUIRenderer.h"
#include "ExampleApplication.h"

CEGUI::MouseButton convertButton(OIS::MouseButtonID buttonID)
{
    switch (buttonID)
    {
    case OIS::MB_Left:
        return CEGUI::LeftButton;

    case OIS::MB_Right:
        return CEGUI::RightButton;

    case OIS::MB_Middle:
        return CEGUI::MiddleButton;

    default:
        return CEGUI::LeftButton;
    }
}

TutorialFrameListener::TutorialFrameListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr)
        : ExampleFrameListener(win, cam, false, false)
    {
        // key and mouse state tracking
        mMouseDown = false;
        mToggle = 0.0;

        // Populate the camera and scene manager containers
        mCamNode = cam->getParentSceneNode();
        mSceneMgr = sceneMgr;

        // set the rotation and move speed
        mRotate = 0.13;
        mMove = 20;

        mContinue=true;
        mMouse->setEventCallback(this);
        mKeyboard->setEventCallback(this);

    }

TutorialFrameListener::~TutorialFrameListener()
{
}

/*
    // MouseListener
bool TutorialFrameListener::mouseMoved(const OIS::MouseEvent &arg)
{
  CEGUI::System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
  return true;
}
*/

bool TutorialFrameListener::frameStarted(const FrameEvent &evt)
{
        mMouse->capture();
        mKeyboard->capture();

        return mContinue && !mKeyboard->isKeyDown(OIS::KC_ESCAPE);
}

bool TutorialFrameListener::mouseMoved(const OIS::MouseEvent &arg)
{
    CEGUI::System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
    return true;
}

bool TutorialFrameListener::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    CEGUI::System::getSingleton().injectMouseButtonDown(convertButton(id));
    return true;
}

bool TutorialFrameListener::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    CEGUI::System::getSingleton().injectMouseButtonUp(convertButton(id));
    return true;
}

bool TutorialFrameListener::keyPressed( const OIS::KeyEvent &arg )
{
    CEGUI::System *sys = CEGUI::System::getSingletonPtr();
    sys->injectKeyDown(arg.key);
    sys->injectChar(arg.text);

    return true;
}
bool TutorialFrameListener::keyReleased( const OIS::KeyEvent &arg )
{
    CEGUI::System::getSingleton().injectKeyUp(arg.key);

    return false;
}


/*
bool TutorialFrameListener::quit(const CEGUI::EventArgs &e)
{
    mContinue = false;
    return true;
}
*/
