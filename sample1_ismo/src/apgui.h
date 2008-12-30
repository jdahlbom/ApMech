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

    protected:
    void _initialize(RenderWindow *window, SceneManager *sceneManager);
    void _setLookAndFeel();
    void _createGUIWindow();
    void _registerEventCallbacks(ApEventHandler *eventHandler);

    private:
    CEGUI::OgreCEGUIRenderer *mRenderer_;
    CEGUI::System *mSystem_;


};


#endif
