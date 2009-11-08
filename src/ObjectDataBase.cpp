#include "ObjectDataBase.h"
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace ap {

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

            std::cout << "Found XML file: " << iter->leaf() << std::endl;
            std::string file = dataPath + "/" + iter->leaf();

            MechReader mechReader(&file);
            mechReader.parseFile();

            std::cout << "name: " << mechReader.getName() << std::endl;
            std::cout << "turnrate: " << mechReader.getTurnRate() << std::endl;
            std::cout << "forward acceleration: " << mechReader.getMaxForwardAcceleration() << std::endl;
            std::cout << "backward acceleration: " << mechReader.getMaxBackwardAcceleration() << std::endl;

            mechMap.insert(std::make_pair<std::string, MechReader>(mechReader.getName(), mechReader));

        }
    }
}

}

#if 0
// test code, compilation:
// g++ -lboost_filesystem-mt -lexpat src/ObjectDataBase.cpp src/ObjectReader.cpp -o test_mechdb

int main() {
    ap::MechDatabase db("data/mechs");

    db.readMechFiles();

    return 0;
}
#endif
