#include "ObjectDataBase.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace ap {

MechDatabase::~MechDatabase(void) {
    // TODO: delete all mechreader instances in the map
}

bool MechDatabase::readMechFiles() {

    std::string ending = ".xml";

    if (!fs::exists("data/mechs"))
        return false;

    fs::directory_iterator end_iter; // by default the "last" file
    fs::directory_iterator iter(dataPath);

    for (; iter != end_iter; ++iter) {
        if (iter->leaf().compare (iter->leaf().length() - ending.length(),
                    ending.length(), ending) == 0) {
            // the file ends with '.xml'

#if 0
            std::cout << "Found XML file: " << iter->leaf() << std::endl;
#endif
            std::string file = dataPath + "/" + iter->leaf();

            MechReader *mechReader = new MechReader(&file);
            mechReader->parseFile();

#if 0
            std::cout << "name: " << mechReader->getName() << std::endl;
            std::cout << "turnrate: " << mechReader->getTurnRate() << std::endl;
            std::cout << "forward acceleration: " << mechReader->getMaxForwardAcceleration() << std::endl;
            std::cout << "backward acceleration: " << mechReader->getMaxBackwardAcceleration() << std::endl;
#endif

            mechMap.insert(std::make_pair<std::string, MechReader*>(mechReader->getName(), mechReader));

        }
    }
}


std::vector<std::string> MechDatabase::getMechNames() {

    std::vector<std::string> keys;
    std::map<std::string, MechReader*>::iterator iter;

    for (iter = mechMap.begin(); iter != mechMap.end(); ++iter)
        keys.push_back(iter->first);

    return keys;
}


}

#if 0
// test code, compilation:
// g++ -lboost_filesystem-mt -lexpat src/ObjectDataBase.cpp src/ObjectReader.cpp -o test_mechdb

int main() {
    ap::MechDatabase db("data/mechs");

    db.readMechFiles();

    std::vector<std::string> mechNames = db.getMechNames();

    for (int i = 0; i < mechNames.size(); i++) {

        ap::MechReader *reader = db.getMechReader(mechNames[i]);

        std::cout << "name: " << reader->getName() << std::endl;
        std::cout << "turnrate: " << reader->getTurnRate() << std::endl;
        std::cout << "forward acceleration: " << reader->getMaxForwardAcceleration() << std::endl;
        std::cout << "backward acceleration: " << reader->getMaxBackwardAcceleration() << std::endl;
    }

    return 0;
}
#endif
