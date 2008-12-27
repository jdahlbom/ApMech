#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <map>
#include <Ogre.h>

class GameObject {

    /* Here we add data about the properties that are common to all
     * objects in the game world */

    // static int refcount;
    static void initMaps();
    static void deinitMaps();

 private:

    Ogre::SceneNode *node;

    // is the object visible in the game world
    bool visible;

    // does the object need syncing with the network server?
    bool dirty;

    // is the player currently controlling this object?
    bool player;

    int x;
    int y;
    int z;

 public:

    int id;

    // maps for changing mapping the game objects to ints and vice versa
    static std::map<GameObject *, int> *go_to_id;
    static std::map<int, GameObject *> *id_to_go;

    std::string id_s; // string representation of the id

    GameObject(int id);

    virtual ~GameObject();

    // any class that inherits GameObject must be ready to serialize!
    virtual int serialize(uint8_t buffer[], int start, int buflength) = 0;

    bool isDirty() { return dirty; }
    void setDirty(bool state) { dirty = state; }
    bool isVisible() { return visible; }
    void setVisible(bool state) { visible = state; }
    bool isPlayer() { return player; }
    void setPlayer(bool state) { player = state; }

    Ogre::SceneNode *getGraphics() { return node; }
    void setGraphics(Ogre::SceneNode *node) { this->node = node; }

    void setLocation(int x, int y, int z) { this->x = x; this->y = y; this->z = z; }
    int getX() { return this->x; }
    int getY() { return this->y; }
    int getZ() { return this->z; }

    // only the world overrides this
    virtual bool isWorld();
};

class GameObjectId {

 private:
    int id;
 public:
    GameObjectId(int id) : id(id) { }
    GameObject * get();
};

#endif
