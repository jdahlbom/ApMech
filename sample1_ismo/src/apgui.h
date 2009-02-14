#ifndef AP_GUI_H
#define AP_GUI_H

#include <CEGUI/CEGUI.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#include <Carbon/Carbon.h>
#endif

#include <Ogre.h>
#include <OgreCEGUIRenderer.h>

using namespace Ogre;

class ApEventHandler;

class ApGui
{
    public:
    ApGui(RenderWindow *window, SceneManager *sceneManager, ApEventHandler *eventHandler);
    virtual ~ApGui();
    CEGUI::System *getSystemPtr() const;
    void toggleMainMenu();

    protected:
    void initialize(RenderWindow *window, SceneManager *sceneManager);
    void setLookAndFeel();
    void createGUIWindow();
    void registerEventCallbacks();

    CEGUI::OgreCEGUIRenderer *mRenderer;
    CEGUI::System *mSystem;
    ApEventHandler *mEventHandler;
    CEGUI::WindowManager *mWin;
    CEGUI::Window *mRoot;

    private:
    bool buttonQuit(const CEGUI::EventArgs &arg);
    bool buttonReturnToGame(const CEGUI::EventArgs &arg);
    bool buttonOptions(const CEGUI::EventArgs &arg);
    bool buttonBack(const CEGUI::EventArgs &arg);
    void switchSheets(const char *fromName, const char *toName);

};


#endif
