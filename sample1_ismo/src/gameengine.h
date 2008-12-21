/*
 *  gameengine.h
 *  sample1_ismo
 *
 *  Created by Ismo Puustinen on 21.12.2008.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "gameworld.h"
#include <iostream>

class GameEngine {

	private:
	// network connection to server

	public:
	
	GameEngine();
	virtual ~GameEngine();
	
	// get a game world from the network server
	bool connectToServer();
	
	// sync the world with the server
	bool syncWorld(GameWorld *);

	bool processKbEvent(int test);

};