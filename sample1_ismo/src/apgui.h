#ifndef AP_GUI_H
#define AP_GUI_H

#include <Ogre.h>
#include <CEGUI/CEGUI.h>
#include <OgreCEGUIRenderer.h>

class ApEventHandler;


using namespace Ogre;

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
    bool quit(const CEGUI::EventArgs &arg);
    bool returnToGame(const CEGUI::EventArgs &arg);

};


#endif
