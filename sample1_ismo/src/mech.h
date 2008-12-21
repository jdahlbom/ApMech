#ifndef MECH_H
#define MECH_H

#include "gameobject.h"
#include "gameworld.h"

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