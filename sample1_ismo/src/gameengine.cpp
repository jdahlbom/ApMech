#include "apmech.h"
#include "apeventhandler.h"

#include <list>

GameEngine::GameEngine(): keepRendering(true)
{
    mEventHandler = new ApEventHandler(this);
}
GameEngine::~GameEngine()
{
    if(mEventHandler)
        delete(mEventHandler);
}

bool GameEngine::connectToServer()
{
    // TODO: request this from the server here

    // FIXME: a hack to simulate the objects sent by server

    GameWorld *gw = new GameWorld(1);
    GameObject *mech = new Mech(2);

    mech->setLocation(Vector3(750, 0, 750));
    mech->setPlayer(true);
    mech->setVisible(true);

    gw->setPlayer(false);
    gw->setVisible(false);

    processNetworkEvent(gw);
    processNetworkEvent(mech);

    return true;
}

bool GameEngine::syncWorld()
{
    std::list<GameObject *>::iterator i;

    if (mWorld == NULL)
        return false;

    std::list<GameObject *> objects = mWorld->getObjects();

    for (i = objects.begin(); i != objects.end(); ++i) {
        GameObject *object = *i;
        if (object->isDirty()) {
            // TODO: queue the object for syncing with the game server here
            object->setDirty(false);
        }
    }

    return true;
}

bool GameEngine::processKbEvent(int key) {

    if (mWorld == NULL || mPlayer == NULL) {
        /* not yet initialized */
        std::cout << "Not initialized!";
        return true;
    }


    mPlayer->setDirty(true);

    // FIXME: a hack to simulate network processing here
    processNetworkEvent(mPlayer);

    return true;
}

bool GameEngine::processNetworkEvent(GameObject *gameObject) {
    /* TODO: put the object to the maps and lists */
    /* TODO: update references to the object */

    if (gameObject->isWorld()) {
        std::cout << "Received a world!" << std::endl;
        mWorld = (GameWorld *) gameObject;
    }

    if (gameObject->isPlayer()) {
        std::cout << "Received a player!" << std::endl;
        mPlayer = gameObject;
    }

    mGraphics->updateGraphics(gameObject);

    return true;
}

bool GameEngine::quitEvent()
{
    this->keepRendering = false;
    return false;
}

bool GameEngine::initialize(Graphics *graphics)
{
    mGraphics = graphics;
    return this->connectToServer();
}

ApEventHandler *GameEngine::getEventHandler() const { return mEventHandler; }

bool GameEngine::run()
{
    while (this->keepRendering) {
        // std::cout << "Render frame " << i++ << "\n";

        mGraphics->render();
    }

    return true;
}
