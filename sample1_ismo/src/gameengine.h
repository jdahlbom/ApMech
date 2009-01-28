#ifndef GAMEENGINE_H
#define GAMEENGINE_H

class Graphics;
class GameObject;
class GameWorld;
class ApEventHandler;

class GameEngine {

 friend class ApEventHandler;

 private:
    // player
    GameObject *mPlayer;
    // world
    GameWorld *mWorld;
    // graphics
    Graphics *mGraphics;
    // network connection to server
    ApEventHandler *mEventHandler;

    bool keepRendering;

    // get a game world from the network server
    bool connectToServer();

 public:

    GameEngine();
    virtual ~GameEngine();

    bool initialize(Graphics *graphics);

    // sync the world with the server
    bool syncWorld();

    bool processKbEvent(int event);

    bool quitEvent();

    bool processNetworkEvent(GameObject *gameObject);
    bool run();

    ApEventHandler *getEventHandler() const;
};
#endif
