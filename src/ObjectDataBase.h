// The Mech data handling algorithm:
//
// 1. Server reads all XML game object files to the ObjectData
//    database.
//
//      Example: File data/mechs/hybridfalcon-15.xml contains data on
//      one mech, which will be represented as an ObjectData
//      instance. This instance will be mapped by the name property
//      (which any and all game object types shall have).
//
// 2. When a client logs in, the server sends a list of all mechs
//    to the client. These are also represented as ObjectData
//    instances.
//
// 2. The client presents a list of mechs in the Limbo screen (or 
//    where ever).
//
// 3. When a new mech is created, associate it with an ObjectData
//    instance from the database. The mech gets the initial values from 
//    the model. It is possible that the values change during the game,
//    but even then the ObjectData instance is unchanged.
//    
// 4. All ObjectData instances are destroyed when the programs are
//    ended, not before.
//

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include "ObjectReader.h"

namespace ap {

class MechDatabase {

public:
    MechDatabase(std::string path) : dataPath(path) {}
    ~MechDatabase(void);

    bool readMechFiles();
    std::vector<std::string> getMechNames();

    // TODO: use shared_ptr? The MechReader pointers are not usable
    // after deleting the database.
    MechReader * getMechReader(std::string name) { return mechMap[name]; }

private:
    // map of ObjectDataModels (name -> object)
    
    std::string dataPath;
    
    std::map<std::string, MechReader*> mechMap;

};
}
