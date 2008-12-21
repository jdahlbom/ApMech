#include "apmech.h"

#include <list>

GameEngine::GameEngine(): keepRendering(true) {}
GameEngine::~GameEngine() {}
	
bool GameEngine::connectToServer()
{
	// TODO: request this from the server here
    
    // FIXME: a hack to simulate the objects sent by server
    
    GameWorld *gw = new GameWorld(1);
    GameObject *mech = new Mech(2);
    
    mech->setLocation(750,750,0);
    mech->set_player(true);
    mech->set_visible(true);
    
    gw->set_player(false);
    gw->set_visible(false);
    
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

    int x, y, z;
    
    // give 'quitting' priority
    if (key == OIS::KC_ESCAPE) {
        this->keepRendering = false;
        return false;
    }    
    
    if (world == NULL || player == NULL) {
        /* not yet initialized */
        return true;
    }
    
    x = player->getX();
    y = player->getY();
    z = player->getZ();
    
	switch (key) {
		case OIS::KC_UP:
			std::cout << "up key\n";
			player->setLocation(x, y+1, z);
			break;
		case OIS::KC_DOWN:
			std::cout << "down key\n";
			player->setLocation(x, y-1, z);
			break;
		case OIS::KC_LEFT:
			std::cout << "left key\n";
			player->setLocation(x-1, y, z);
			break;
		case OIS::KC_RIGHT:
			std::cout << "right key\n";
			player->setLocation(x+1, y, z);
			break;
		default:
			std::cout << "unknown key\n";
	}
    
    player->setDirty(true);
    
    // FIXME: a hack to simulate network processing here
    processNetworkEvent(player);
    
	return true;
}

bool GameEngine::processNetworkEvent(GameObject *o) {
    /* TODO: put the object to the maps and lists */
    /* TODO: update references to the object */
    
    if (o->isWorld()) {
        std::cout << "Received a world!" << std::endl;
        this->world = (GameWorld *) o;
    }
    
    if (o->is_player()) {
        std::cout << "Received a player!" << std::endl;
        this->player = o;
    }
    
    graphics->updateGraphics(o);
    
    return true;
}


bool GameEngine::initialize(Graphics *graphics)
{
    this->graphics = graphics;
    return this->connectToServer();
}

bool GameEngine::run()
{
	while (this->keepRendering) {
		// std::cout << "Render frame " << i++ << "\n";
        
		graphics->render(); 
	}
    
    return true;
}