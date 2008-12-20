#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <list>
#include "gameobject.h"

class GameWorld: public GameObject {
    
    private:
    // list of all objects in the game world
    std::list<GameObject *> world_objects;

    // dimensions

    // reference to the actual 3D representation of the world

    public:
    GameWorld();
    virtual GameWorld::~GameWorld();

    void addObject(GameObject *o);
	bool removeObject(GameObject *o);
};
#endif