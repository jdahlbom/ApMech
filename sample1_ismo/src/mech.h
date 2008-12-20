#ifndef MECH_H
#define MECH_H

#include "gameobject.h"
#include "gameworld.h"

class Mech: public GameObject {
    
    private:
    GameWorld *world;
    // Location *location;

	int x;
	int y;

    public:
	
    Mech();
	virtual ~Mech();

	void setLocation(int x, int y);
	int getX();
	int getY();
};
#endif