class GameObject {
    
    /* Here we add data about the properties that are common to all
     * objects in the game world */

    // is the object visible in the game world
    bool visible;

    // is it possible to serialize this object for network transfer
    bool serializable;

    public:

    GameObject(): visible(false), serializable(true) {}

    virtual ~GameObject() {}

};
