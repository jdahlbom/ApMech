#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <map>
#include <Ogre.h>

using namespace Ogre;

class GameObject {

    /* Here we add data about the properties that are common to all
     * objects in the game world */

    // static int refcount;
    static void initMaps();
    static void deinitMaps();

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

    bool isDirty() const;
    void setDirty(bool state);
    bool isVisible() const;
    void setVisible(bool state);
    bool isPlayer() const;
    void setPlayer(bool state);

    Ogre::SceneNode *getSceneNode() const;
    void setSceneNode(Ogre::SceneNode *node);

    void setLocation(Vector3 location);
    Vector3 getLocation() const;

    // only the world overrides this
    virtual bool isWorld() const;

 protected:
    Ogre::SceneNode *node;

    // is the object visible in the game world
    bool visible;

    // does the object need syncing with the network server?
    bool dirty;

    // is the player currently controlling this object?
    bool player;

    Ogre::Vector3 location_;
    Ogre::Vector3 velocity_;

};

class GameObjectId {

 private:
    int id;
 public:
    GameObjectId(int id) : id(id) { }
    GameObject * get();
};

#endif
