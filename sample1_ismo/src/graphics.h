#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Ogre.h>


using namespace Ogre;

class GameEngine;
class GameObject;
class GameWorld;

class Graphics {

 private:

    // for Ogre 3D
    Root *root_;
    RenderWindow *window_;
    RenderSystem *rSys_;
    SceneManager *sceneMgr_;

    // for game logic
    GameWorld *world_;
    GameEngine *engine_;

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
