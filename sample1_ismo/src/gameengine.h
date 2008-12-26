#ifndef GAMEENGINE_H
#define GAMEENGINE_H

class Graphics;
class GameObject;
class GameWorld;

class GameEngine {

    private:
    // player
    GameObject *player;
    // world
    GameWorld *world;
    // graphics
    Graphics *graphics;
    // network connection to server

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
    bool processNetworkEvent(GameObject *o);
    bool run();

};
#endif
