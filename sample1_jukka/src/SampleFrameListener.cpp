#include "SampleFrameListener.h"

#include "ExampleApplication.h"

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
        mMove = 250;

    }

    // Overriding the default processUnbufferedKeyInput so the key updates we define
    // later on work as intended.
bool TutorialFrameListener::processUnbufferedKeyInput(const FrameEvent& evt)
{
    return true;
}

    // Overriding the default processUnbufferedMouseInput so the Mouse updates we define
    // later on work as intended.
bool TutorialFrameListener::processUnbufferedMouseInput(const FrameEvent& evt)
{
    return true;
}

bool TutorialFrameListener::frameStarted(const FrameEvent &evt)
{
        mMouse->capture();
        mKeyboard->capture();

        if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
           return false;

        return true;
}
