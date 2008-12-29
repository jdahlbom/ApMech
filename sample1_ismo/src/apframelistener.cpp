#include "apframelistener.h"

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>

#include "apgui.h"
#include "gameengine.h"

ApFrameListener::ApFrameListener(RenderWindow *window,
                                GameEngine *engine,
                                CEGUI::System *guiSystem) :
                                    engine_(engine), window_(window),
                                    guiSystem_(guiSystem)
{
    this->_initializeOIS(window);
}

ApFrameListener::~ApFrameListener()
{}

void ApFrameListener::_initializeOIS(RenderWindow *window)
{
    // using buffered input
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    window_->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager_ = OIS::InputManager::createInputSystem( pl );
    mKeyboard_ = static_cast<OIS::Keyboard*>(mInputManager_->createInputObject( OIS::OISKeyboard, true ));
    // mInputManager->addKeyListener(this, "game");
    mKeyboard_->setEventCallback(this);
}

bool ApFrameListener::keyPressed(const OIS::KeyEvent &arg)
{
    std::cout << "Key pressed!\n";

    guiSystem_->injectKeyDown(arg.key);
    guiSystem_->injectChar(arg.text);

    return engine_->processKbEvent(arg.key);
}


bool ApFrameListener::keyReleased(const OIS::KeyEvent &arg)
{
    guiSystem_->injectKeyDown(arg.key);
    guiSystem_->injectChar(arg.text);

    return true;
}


bool ApFrameListener::mouseMoved(const OIS::MouseEvent &arg)
{
    CEGUI::System::getSingleton().injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
    return true;
}


bool ApFrameListener::mousePressed(const OIS::MouseEvent &arg,
                                    OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().injectMouseButtonDown(_convertButton(id));
    return true;
}


bool ApFrameListener::mouseReleased(const OIS::MouseEvent &arg,
                                    OIS::MouseButtonID id)
{
    CEGUI::System::getSingleton().injectMouseButtonUp(_convertButton(id));
    return true;
}


bool ApFrameListener::frameStarted(const FrameEvent &evt)
{
    // std::cout << "frame!\n";
    mKeyboard_->capture();
    return true;
}


bool ApFrameListener::quit(const CEGUI::EventArgs&)
{
    return engine_->quitEvent();
}


CEGUI::MouseButton ApFrameListener::_convertButton(OIS::MouseButtonID buttonID)
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

