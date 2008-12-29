#include "apmech.h"
#include "apeventhandler.h"

#include <list>

GameEngine::GameEngine(): keepRendering(true)
{
    this->eventHandler_ = new ApEventHandler(this);
}
GameEngine::~GameEngine()
{
    if(this->eventHandler_)
        delete(this->eventHandler_);
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

    if (world == NULL)
        return false;

    std::list<GameObject *> objects = this->world->getObjects();

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

    // give 'quitting' priority
    if (key == OIS::KC_ESCAPE) {
        this->keepRendering = false;
        return false;
    }

    if (world == NULL || player == NULL) {
        /* not yet initialized */
        return true;
    }

    Vector3 playerLocation = player->getLocation();
    float x=0.0, z=0.0;

    switch (key) {
        case OIS::KC_UP:
            std::cout << "up key" << std::endl;
            x += 1.0;
            break;
        case OIS::KC_DOWN:
            std::cout << "down key" << std::endl;
            x += -1.0;
            break;
        case OIS::KC_LEFT:
            std::cout << "left key" << std::endl;
            z += 1.0;
            break;
        case OIS::KC_RIGHT:
            std::cout << "right key" << std::endl;
            z += -1.0;
            break;
        default:
            //std::cout << "unknown key\n";
            break;
    }
    player->setLocation(playerLocation + Vector3(Real(x),Real(0.0), Real(z)));

    player->setDirty(true);

    // FIXME: a hack to simulate network processing here
    processNetworkEvent(player);

    return true;
}

bool GameEngine::processNetworkEvent(GameObject *gameObject) {
    /* TODO: put the object to the maps and lists */
    /* TODO: update references to the object */

    if (gameObject->isWorld()) {
        std::cout << "Received a world!" << std::endl;
        this->world = (GameWorld *) gameObject;
    }

    if (gameObject->isPlayer()) {
        std::cout << "Received a player!" << std::endl;
        this->player = gameObject;
    }

    graphics->updateGraphics(gameObject);

    return true;
}

bool GameEngine::quitEvent()
{
    this->keepRendering = false;
    return false;
}

bool GameEngine::initialize(Graphics *graphics)
{
    this->graphics = graphics;
    return this->connectToServer();
}

ApEventHandler *GameEngine::getEventHandler() const { return this->eventHandler_; }

bool GameEngine::run()
{
    while (this->keepRendering) {
        // std::cout << "Render frame " << i++ << "\n";

        graphics->render();
    }

    return true;
}
