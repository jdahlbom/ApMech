#include "utilities.h"

#include <string>

#include "../MechData.h"
#include "../ObjectDataBase.h"
#include "../ObjectReader.h"

namespace ap {
namespace server {

  /**
   * Creates a mech data object and populates its data from given database.
   * This MechData object must be deleted in the netdata, not here!
   */
    ap::MechData *createMechData(ap::MechDatabase *mechDB, std::string mechname)
    {
        ap::MechData *data = new ap::MechData();

        // copy the data in place to a netobject
        ap::MechReader *reader = mechDB->getMechReader(mechname);

        data->setName(reader->getName());
        data->setTurnRate(reader->getTurnRate());
        data->setMaxForwardAcceleration(reader->getMaxForwardAcceleration());
        data->setMaxBackwardAcceleration(reader->getMaxBackwardAcceleration());
        data->setMaxSpeed(reader->getMaxSpeed());
	data->setTorsoMesh(reader->getTorsoMesh());
	data->setLegsMesh(reader->getLegsMesh());

        return data;
    }

} // server
} // ap
