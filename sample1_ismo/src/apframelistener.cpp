#include "apframelistener.h"

#include <Ogre.h>
#include <OIS/OIS.h>

#include "gameengine.h"

ApFrameListener::ApFrameListener(RenderWindow *window, GameEngine *engine): engine_(engine), window_(window)
{
    // using buffered input
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    window_->getCustomAttribute("test window", &windowHnd);
    windowHndStr << windowHnd;
    std::cout << "Window handler: " << windowHndStr << "\n";
    exit;
    pl.insert(std::make_pair(std::string("test window"), windowHndStr.str()));

    mInputManager_ = OIS::InputManager::createInputSystem( pl );
    mKeyboard_ = static_cast<OIS::Keyboard*>(mInputManager_->createInputObject( OIS::OISKeyboard, true ));
    // mInputManager->addKeyListener(this, "game");
    mKeyboard_->setEventCallback(this);
}

bool ApFrameListener::keyPressed(const OIS::KeyEvent &arg)
{
    std::cout << "Key pressed!\n";

    return engine_->processKbEvent(arg.key);
}

bool ApFrameListener::keyReleased(const OIS::KeyEvent &arg)
{
    return true;
}

bool ApFrameListener::frameStarted(const FrameEvent &evt)
{
    // std::cout << "frame!\n";
    mKeyboard_->capture();
    return true;
}
