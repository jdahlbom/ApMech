#include "apgui.h"

#include <assert.h>

#include <Ogre.h>
#include <CEGUI/CEGUI.h>

#include "apeventhandler.h"

ApGui::ApGui(RenderWindow *window, SceneManager *sceneManager, ApEventHandler *eventHandler)
{
    assert( window );
    assert( sceneManager );
    assert( eventHandler );

    _initialize(window, sceneManager);
    _setLookAndFeel();
    _createGUIWindow();

    _registerEventCallbacks(eventHandler);
}

ApGui::~ApGui()
{
    if(this->mRenderer_)
        delete(this->mRenderer_);
    if(this->mSystem_)
        delete(this->mSystem_);
}

CEGUI::System *ApGui::getSystemPtr() const { return mSystem_; }

//-----------------------------------------------------------------------------

void ApGui::_initialize(RenderWindow *window, SceneManager *sceneManager)
{
    mRenderer_ = new CEGUI::OgreCEGUIRenderer( window,
                                Ogre::RENDER_QUEUE_OVERLAY,
                                false, 0, sceneManager);

    mSystem_ = new CEGUI::System( mRenderer_ );
}

void ApGui::_setLookAndFeel()
{
    CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");

    mSystem_->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
    mSystem_->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
}

void ApGui::_createGUIWindow()
{
    CEGUI::WindowManager *win = CEGUI::WindowManager::getSingletonPtr();
    CEGUI::Window *sheet = win->createWindow("DefaultGUISheet", "CEGUIDemo/Sheet");

    CEGUI::Window *quit = win->createWindow("TaharezLook/Button", "CEGUIDemo/QuitButton");
    quit->setText("Quit");
    quit->setSize(CEGUI::UVector2(CEGUI::UDim(0.15, 0), CEGUI::UDim(0.05, 0)));

    sheet->addChildWindow(quit);
    mSystem_->setGUISheet(sheet);
}

void ApGui::_registerEventCallbacks(ApEventHandler *eventHandler)
{
    CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();
    CEGUI::Window *quit = wmgr->getWindow((CEGUI::utf8*)"CEGUIDemo/QuitButton");

    quit->subscribeEvent(CEGUI::PushButton::EventClicked,
                    CEGUI::Event::Subscriber(&ApEventHandler::quit, eventHandler));
}
