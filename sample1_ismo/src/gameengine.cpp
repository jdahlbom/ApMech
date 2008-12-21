/*
 *  gameengine.cpp
 *  sample1_ismo
 *
 *  Created by Ismo Puustinen on 21.12.2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "gameengine.h"
#include <OIS/OIS.h>

GameEngine::GameEngine() {}
GameEngine::~GameEngine() {}
	
bool GameEngine::connectToServer()
{
	// TODO: request this from the server here
	
	return true;
	// return new GameWorld(1);
}

bool GameEngine::syncWorld(GameWorld * world)
{
	std::list<GameObject *>::iterator i;

	std::list<GameObject *> objects = world->getObjects();

    for (i = objects.begin(); i != objects.end(); ++i) {
		GameObject *object = *i;
        if (object->is_dirty()) {
		
			// TODO: queue the object for syncing with the game server here
			
			object->set_dirty(false);
        }
    }

	return true;
}

bool GameEngine::processKbEvent(int key) {
			
	// int x = mech.getX();
	// int y = mech.getY();
		
	switch (key) {
		case OIS::KC_UP:
			std::cout << "up key\n";
			// mech.setLocation(x, y+1, 0);
			break;
		case OIS::KC_DOWN:
			std::cout << "down key\n";
			// mech.setLocation(x, y-1, 0);
			break;
		case OIS::KC_LEFT:
			std::cout << "left key\n";
			// mech.setLocation(x-1, y ,0);
			break;
		case OIS::KC_RIGHT:
			std::cout << "right key\n";
			// mech.setLocation(x+1, y, 0);
			break;
		default:
			std::cout << "unknown key\n";
	}
		
	return true;
}