#include "gameworld.h"

GameWorld::GameWorld(int id) : GameObject(id) { }

GameWorld::~GameWorld()
{
    // remove the world_objects vector contents?
}

void GameWorld::addObject(GameObject *o)
{
    world_objects.push_back(o);
    return;
}

bool GameWorld::removeObject(GameObject *o)
{

    std::list<GameObject *>::iterator i;

    for (i = this->world_objects.begin(); i != this->world_objects.end(); ++i) {
        if (*i == o) {
            this->world_objects.erase(i);    
            return true;
        }
    }

    return false;
}

std::list<GameObject *> GameWorld::getObjects()
{
	return this->world_objects;
}

int GameWorld::serialize(uint8_t buffer[], int start, int buflength) {
    /* TODO */
	return 0;
}

bool GameWorld::isWorld()
{
    std::cout << "Is a game world!" << std::endl; 
    return true;
}

#if 0
GameObject * GameWorld::getPlayer()
{
    return this->player;
}

void GameWorld::setPlayer(GameObject *o)
{
    this->player = o;
}
#endif