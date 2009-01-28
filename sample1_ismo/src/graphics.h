#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Ogre.h>


using namespace Ogre;

class GameEngine;
class GameObject;
class GameWorld;
class ApGui;

class Graphics {

 private:

    // for Ogre 3D
    Root *mRoot;
    RenderWindow *mWindow;
    RenderSystem *mRenderSystem;
    SceneManager *mSceneMgr;

    // for game logic
    GameWorld *mWorld;
    GameEngine *mEngine;

    // for GUI
    ApGui *mGuiOverlay;

    void setupResources(void);
    bool loadResources(void);
    bool loadTerrain();


 public:

    Graphics();
    virtual ~Graphics();

    bool initialize(GameEngine *engine);
    bool run();
    bool updateGraphics(GameObject *o);
    bool load();
    bool render();

};

#endif
