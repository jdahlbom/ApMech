class GameWorld: public GameObject {
    
    private:
    // list of all objects in the game world
    std::vector<GameObject> *objects;

    // dimensions

    // reference to the actual 3D representation of the world

    public:
    GameWorld(): GameObject();
};
