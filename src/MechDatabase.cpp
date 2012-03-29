#include "MechDatabase.h"
#include <boost/filesystem.hpp>

#include "MechData.h"
#include "MechReader.h"

namespace fs = boost::filesystem;

namespace ap {

MechDatabase::~MechDatabase(void) {

    std::map<std::string, MechData*>::iterator iter;

    for (iter = mechMap.begin(); iter != mechMap.end(); ++iter)
        delete iter->second;
}

  /**
   * @return True if reading succeeded.
   */
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
            const std::string file = dataPath + "/" + iter->leaf();

		MechData *mechData = new MechData();
		int result = ap::mechreader::readMech(mechData, &file);
		if (result)
			return false;

#if 1
            std::cout << "name: " << mechData->getName() << std::endl;
            std::cout << "turnrate: " << mechData->getTurnRate() << std::endl;
            std::cout << "forward acceleration: " << mechData->getMaxForwardAcceleration() << std::endl;
            std::cout << "backward acceleration: " << mechData->getMaxBackwardAcceleration() << std::endl;
#endif

            mechMap.insert(std::make_pair<std::string, MechData*>(mechData->getName(), mechData));

        }
    }
    return true;
}


std::vector<std::string> MechDatabase::getMechNames() {

    std::vector<std::string> keys;
    std::map<std::string, MechData*>::iterator iter;

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
