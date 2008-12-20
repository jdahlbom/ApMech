#include "ExampleApplication.h"

class TutorialFrameListener : public ExampleFrameListener
{
public:
    TutorialFrameListener(RenderWindow* win, Camera* cam, SceneManager *sceneMgr);

    // Overriding the default processUnbufferedKeyInput so the key updates we define
    // later on work as intended.
    bool processUnbufferedKeyInput(const FrameEvent& evt);

    // Overriding the default processUnbufferedMouseInput so the Mouse updates we define
    // later on work as intended.
    bool processUnbufferedMouseInput(const FrameEvent& evt);

    bool frameStarted(const FrameEvent &evt);

protected:
    bool mMouseDown;       // Whether or not the left mouse button was down last frame
    Real mToggle;          // The time left until next toggle
    Real mRotate;          // The rotate constant
    Real mMove;            // The movement constant
    SceneManager *mSceneMgr;   // The current SceneManager
    SceneNode *mCamNode;   // The SceneNode the camera is currently attached to

};

