/*
 *  gameengine.h
 *  sample1_ismo
 *
 *  Created by Ismo Puustinen on 21.12.2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "gameworld.h"

class GameEngine {

	private:
	// network connection to server

	public:
	
	GameEngine();
	virtual ~GameEngine();
	
	// get a game world from the network server
	GameWorld * request_world();
	
	// sync the world with the server
	bool sync_world(GameWorld *);

};