#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Ogre.h>
#include <OIS/OIS.h>

using namespace Ogre;

class GameEngine;
class GameObject;
class GameWorld;

class MyFrameListener : public FrameListener, public OIS::KeyListener
{
    private:

    GameEngine *engine;
    RenderWindow *window;

    protected:

    OIS::Keyboard *mKeyboard;
    OIS::InputManager* mInputManager;

    public:

    MyFrameListener(RenderWindow *window, GameEngine *engine);

    virtual ~MyFrameListener() {}

    bool keyPressed(const OIS::KeyEvent &arg);
    bool keyReleased(const OIS::KeyEvent &arg);
    bool frameStarted(const FrameEvent &evt);
};

class Graphics {

    private:

    // for Ogre 3D
    Root *root;
    RenderWindow *window;
    RenderSystem *rSys;
    SceneManager *sceneMgr;

    // for game logic
    GameWorld *world;
    GameEngine *engine;

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
