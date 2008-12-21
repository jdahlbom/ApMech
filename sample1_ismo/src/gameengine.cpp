/*
 *  gameengine.cpp
 *  sample1_ismo
 *
 *  Created by Ismo Puustinen on 21.12.2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "gameengine.h"

GameEngine::GameEngine() {}
GameEngine::~GameEngine() {}
	
GameWorld * GameEngine::request_world()
{
	// TODO: request this from the server here
	
	return new GameWorld();
}

bool GameEngine::sync_world(GameWorld * world)
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
