#include "apframelistener.h"

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI/CEGUI.h>

#include "apeventhandler.h"

ApFrameListener::ApFrameListener(RenderWindow *window,
                                ApEventHandler *handler) :
                                    mHandler(handler), mWindow(window)
{
    mGuiSystem = CEGUI::System::getSingletonPtr();
    this->initializeOIS(window);
}

ApFrameListener::~ApFrameListener()
{}

void ApFrameListener::initializeOIS(RenderWindow *window)
{
    // using buffered input
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
/*
    * http://www.ogre3d.org/wiki/index.php/Using_OIS
    *if defined OIS_WIN32_PLATFORM
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND" )));
    pl.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
    pl.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
    *elif defined OIS_LINUX_PLATFORM
    pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
    pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
    *elif defined MAC_OSX_EXAMPLE_WAS_MISSING___BOOHOO_t_Jukka_
    *endif
*/

    mInputManager = OIS::InputManager::createInputSystem( pl );
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    // mInputManager->addKeyListener(this, "game");
    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);
}

bool ApFrameListener::keyPressed(const OIS::KeyEvent &arg)
{
    if (mGuiSystem->injectKeyDown(arg.key))
    {
        if(mGuiSystem->injectChar(arg.text))
            return true;
    }

    // TODO: Siirretään omaan luokkaansa, ehkä?
    switch (arg.key)
    {
        case OIS::KC_ESCAPE:
            mHandler->toggleMainMenu();
            break;
        case OIS::KC_UP:
            mHandler->moveUp();
            break;
        case OIS::KC_DOWN:
            mHandler->moveDown();
            break;
        case OIS::KC_LEFT:
            mHandler->moveLeft();
            break;
        case OIS::KC_RIGHT:
            mHandler->moveRight();
            break;
        default:
            break;
    }

    //return mEngine->processKbEvent(arg.key);
    return false;
}


bool ApFrameListener::keyReleased(const OIS::KeyEvent &arg)
{
    mGuiSystem->injectKeyDown(arg.key);
    mGuiSystem->injectChar(arg.text);

    return true;
}


bool ApFrameListener::mouseMoved(const OIS::MouseEvent &arg)
{
    mGuiSystem->injectMouseMove(arg.state.X.rel, arg.state.Y.rel);
    return true;
}


bool ApFrameListener::mousePressed(const OIS::MouseEvent &arg,
                                    OIS::MouseButtonID id)
{
    mGuiSystem->injectMouseButtonDown(convertButton(id));
    return true;
}


bool ApFrameListener::mouseReleased(const OIS::MouseEvent &arg,
                                    OIS::MouseButtonID id)
{
    mGuiSystem->injectMouseButtonUp(convertButton(id));
    return true;
}


bool ApFrameListener::frameStarted(const FrameEvent &evt)
{
    // std::cout << "frame!\n";
    mMouse->capture();
    mKeyboard->capture();

    mHandler->move(evt.timeSinceLastFrame);
    return true;
}


/**!
 * Muunnosfunktio, joka kääntää OIS:n hiiri-ID:t CEGUI:n käyttämiksi.
 * Varoituksena: tuo switchin default-arvona palautettava LeftButton voi aiheutua vielä ongelmaksi.
 *
 * @param buttonID OIS::MouseButtonID Painettu hiirennappi.
 * @return CEGUI::MouseButton Sama hiirennapin id CEGUIn ymmärtämässä muodossa
 */
CEGUI::MouseButton ApFrameListener::convertButton(OIS::MouseButtonID buttonID)
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
