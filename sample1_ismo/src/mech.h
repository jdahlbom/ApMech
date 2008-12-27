#ifndef MECH_H
#define MECH_H

class GameWorld;

#include "gameobject.h"

class Mech: public GameObject {
    
 private:
    GameWorld *world;
    // Location *location;

 public:
	
    Mech(int id);
	virtual ~Mech();

	int serialize(uint8_t buffer[], int start, int buflength);
};
#endif
