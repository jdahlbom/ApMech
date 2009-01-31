#include "apgui.h"

#include <assert.h>

#include <Ogre.h>
#include <CEGUI/CEGUI.h>

#include "apeventhandler.h"

ApGui::ApGui(RenderWindow *window,
            SceneManager *sceneManager,
            ApEventHandler *eventHandler) :
                mEventHandler(eventHandler)
{
    assert( window );
    assert( sceneManager );
    assert( eventHandler );

    mEventHandler->setGui(this);

    initialize(window, sceneManager);
    setLookAndFeel();
    createGUIWindow();

    registerEventCallbacks();
}

ApGui::~ApGui()
{
    if(mRenderer)
        delete(mRenderer);
    if(mSystem)
        delete(mSystem);
}

CEGUI::System *ApGui::getSystemPtr() const { return mSystem; }

//-----------------------------------------------------------------------------

void ApGui::initialize(RenderWindow *window, SceneManager *sceneManager)
{
    mRenderer = new CEGUI::OgreCEGUIRenderer( window,
                                Ogre::RENDER_QUEUE_OVERLAY,
                                false, 0, sceneManager);

    mSystem = new CEGUI::System( mRenderer );
}

void ApGui::setLookAndFeel()
{
    CEGUI::SchemeManager::getSingleton().loadScheme((CEGUI::utf8*)"TaharezLookSkin.scheme");

    mSystem->setDefaultMouseCursor((CEGUI::utf8*)"TaharezLook", (CEGUI::utf8*)"MouseArrow");
    mSystem->setDefaultFont((CEGUI::utf8*)"BlueHighway-12");
}

void ApGui::createGUIWindow()
{
    mWin = CEGUI::WindowManager::getSingletonPtr();
    mRoot = mWin->createWindow("DefaultGUISheet", "root");

    CEGUI::FrameWindow *menuWindow = (CEGUI::FrameWindow *) mWin->createWindow("TaharezLook/FrameWindow", "root/menuWindow");
    menuWindow->deactivate();
    menuWindow->setVisible(false);
    mRoot->addChildWindow(menuWindow);

    menuWindow->setSize(CEGUI::UVector2(CEGUI::UDim(0.5, 0), CEGUI::UDim(0.75, 0)));
    menuWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.25, 0), CEGUI::UDim(0.125, 0)));
    menuWindow->setText("Main menu");

    CEGUI::UVector2 buttonSize = CEGUI::UVector2(CEGUI::UDim(0.6, 0), CEGUI::UDim(0.1, 0));

    // Menu main page

    CEGUI::Window *menuMainPage = mWin->createWindow("DefaultGUISheet", "root/menuWindow/mainpage");
    menuWindow->addChildWindow(menuMainPage);

    CEGUI::Window *mainOptions = mWin->createWindow("TaharezLook/Button", "root/menuWindow/mainpage/optionsButton");
    mainOptions->setText("Options");
    mainOptions->setSize(buttonSize);
    mainOptions->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.2, 0)));

    CEGUI::Window *mainReturn = mWin->createWindow("TaharezLook/Button", "root/menuWindow/mainpage/returnButton");
    mainReturn->setText("Return to game");
    mainReturn->setSize(buttonSize);
    mainReturn->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.4, 0)));

    CEGUI::Window *mainQuit = mWin->createWindow("TaharezLook/Button", "root/menuWindow/mainpage/quitButton");
    mainQuit->setText("Quit");
    mainQuit->setSize(buttonSize);
    mainQuit->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.6, 0)));

    menuMainPage->addChildWindow(mainOptions);
    menuMainPage->addChildWindow(mainReturn);
    menuMainPage->addChildWindow(mainQuit);


    // Menu Options

    CEGUI::Window *menuOptions = mWin->createWindow("DefaultGUISheet", "root/menuWindow/options");
    menuWindow->addChildWindow(menuOptions);
    menuOptions->hide();

    CEGUI::Window *optionsControls = mWin->createWindow("TaharezLook/Button", "root/menuWindow/options/controlsButton");
    optionsControls->setText("Controls");
    optionsControls->setSize(buttonSize);
    optionsControls->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.2, 0)));

    CEGUI::Window *optionsBack = mWin->createWindow("TaharezLook/Button", "root/menuWindow/options/backButton");
    optionsBack->setText("Back");
    optionsBack->setSize(buttonSize);
    optionsBack->setPosition(CEGUI::UVector2(CEGUI::UDim(0.2, 0), CEGUI::UDim(0.6, 0)));

    menuOptions->addChildWindow(optionsControls);
    menuOptions->addChildWindow(optionsBack);

    mSystem->setGUISheet(mRoot);
}

void ApGui::registerEventCallbacks()
{
    CEGUI::WindowManager *wmgr = CEGUI::WindowManager::getSingletonPtr();

    CEGUI::Window *quit = wmgr->getWindow((CEGUI::utf8*)"root/menuWindow/mainpage/quitButton");
    quit->subscribeEvent(CEGUI::PushButton::EventClicked,
                        CEGUI::Event::Subscriber(&ApGui::buttonQuit, this));

    CEGUI::Window *returnToGame = wmgr->getWindow((CEGUI::utf8*)"root/menuWindow/mainpage/returnButton");
    returnToGame->subscribeEvent(CEGUI::PushButton::EventClicked,
                        CEGUI::Event::Subscriber(&ApGui::buttonReturnToGame, this));

    CEGUI::Window *options = wmgr->getWindow((CEGUI::utf8*)"root/menuWindow/mainpage/optionsButton");
    options->subscribeEvent(CEGUI::PushButton::EventClicked,
                        CEGUI::Event::Subscriber(&ApGui::buttonOptions, this));

    CEGUI::Window *back = wmgr->getWindow((CEGUI::utf8*)"root/menuWindow/options/backButton");
    back->subscribeEvent(CEGUI::PushButton::EventClicked,
                        CEGUI::Event::Subscriber(&ApGui::buttonBack, this));
}

/**
 * Handles pushing the Quit- button.
 * @param arg CEGUI::EventArgs* Not used, but required for a proper callback function
 * @return bool If false, the event will be propagated further on within the GUI. If true,
 *              this event handling stops here.
 */
bool ApGui::buttonQuit(const CEGUI::EventArgs &arg)
{
    mEventHandler->quit();
    return true;
}

/**
 * Handles pushing the Return to Game- button
 * @param arg CEGUI::EventArgs* Unused
 * @return bool If false, the event will be propagated further on within the GUI. If true,
 *              this event handling stops here.
 */
bool ApGui::buttonReturnToGame(const CEGUI::EventArgs &arg)
{
    toggleMainMenu();
    return true;
}


/**
 * Handles pushing the Options- button
 * @param arg CEGUI::EventArgs* Unused
 * @return bool If false, the event will be propagated further on within the GUI. If true,
 *              this event handling stops here.
 */
bool ApGui::buttonOptions(const CEGUI::EventArgs &arg)
{

    const char* mainSheet = "root/menuWindow/mainpage";
    const char* optionsSheet = "root/menuWindow/options";
    switchSheets(mainSheet, optionsSheet);

    return true;
}


/**
 * Handles pushing the Back- button
 * @param arg CEGUI::EventArgs* Unused
 * @return bool If false, the event will be propagated further on within the GUI. If true,
 *              this event handling stops here.
 */
bool ApGui::buttonBack(const CEGUI::EventArgs &arg)
{
    const char* optionsSheet = "root/menuWindow/options";
    const char* mainSheet = "root/menuWindow/mainpage";
    switchSheets(optionsSheet, mainSheet);

    return true;
}

/**
 * Manages switching between menu sheets
 * @param fromName const char * Name of the sheet from which we are changing out of
 * @param toName const char * Name of the sheet we are changing to
 */
void ApGui::switchSheets(const char *fromName, const char *toName)
{
    CEGUI::Window *previousSheet = mRoot->getChildRecursive(fromName);
    CEGUI::Window *newSheet = mRoot->getChildRecursive(toName);
    assert( 0 != previousSheet );
    assert( 0 != newSheet );

    previousSheet->hide();
    newSheet->show();
    newSheet->activate();
}

void ApGui::toggleMainMenu()
{
    const char* menuWindowName = "root/menuWindow";

    CEGUI::Window *menuWindow = mRoot->getChild(menuWindowName);

    assert( 0 != menuWindow );


    if( menuWindow->isActive() ) {
        // Need to switch the shown sheet back to main menu
        menuWindow->hide();
    } else {
        menuWindow->show();
        menuWindow->activate();
    }
}
