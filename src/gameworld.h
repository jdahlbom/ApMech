#include <list>

using std::list;

class GameWorld: public GameObject {
    
    private:
    // list of all objects in the game world
    std::list<GameObject *> world_objects;

    // dimensions

    // reference to the actual 3D representation of the world

    public:
    GameWorld(): GameObject();
    virtual GameWorld::~GameWorld();

    void addObject(GameObject *o);
};
