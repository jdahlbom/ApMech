#include <map>
#include <Ogre.h>

#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject {
    
	/* Here we add data about the properties that are common to all
     * objects in the game world */
	
	// maps for changing mapping the game objects to ints and vice versa

	static std::map<GameObject *, int> *go_to_id;
	static std::map<int, GameObject *> *id_to_go;
	// static int refcount;
	static void init_maps();
	static void deinit_maps();
	
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
	std::string id_s; // string representation of the id

    GameObject(int id);

    virtual ~GameObject();

	// any class that inherits GameObject must be ready to serialize!
	virtual int serialize(uint8_t buffer[], int start, int buflength) = 0;

	bool is_dirty() { return dirty; }
	void set_dirty(bool state) { dirty = state; }
	bool is_visible() { return visible; }
	void set_visible(bool state) { visible = state; }
	bool is_player() { return player; }
	void set_player(bool state) { player = state; }	
	
	Ogre::SceneNode *getGraphics() { return node; }
	void setGraphics(Ogre::SceneNode *node) { this->node = node; }


	
	void setLocation(int x, int y, int z) { this->x = x; this->y = y; this->z = z; }
	int getX() { return this->x; }
	int getY() { return this->y; }
	int getZ() { return this->z; }

};

#endif
