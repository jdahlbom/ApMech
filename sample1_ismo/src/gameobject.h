#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

class GameObject {
    
    /* Here we add data about the properties that are common to all
     * objects in the game world */

    // is the object visible in the game world
    bool visible;

    // is it possible to serialize this object for network transfer
    bool serializable;
	
	// does the object need syncing with the network server?
	bool dirty;
	
	int x;
	int y;
	int z;

    public:

    GameObject(): visible(false), serializable(true) {}

    virtual ~GameObject() {}
	
	bool is_dirty() { return dirty; }
	void set_dirty(bool state) { dirty = state; }
	
	virtual int serialize(uint8_t buffer[], int start, int buflength) = 0;
	
	void setLocation(int x, int y, int z) { this->x = x; this->y = y; this->z = z; }

	int getX() { return this->x; }
	int getY() { return this->y; }
	int getZ() { return this->z; }


};
#endif