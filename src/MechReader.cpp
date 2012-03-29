#include "MechReader.h"
#include <stdlib.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <OgreVector3.h>

#include "MechData.h"

#include "model/datamodelStub.h"
#include "model/genmech.nsmap"

namespace ap {
namespace mechreader {
int convertXmlDataToMechData(MechData *data, const _genmech__mech *xmlmech) {

}

int readMech(MechData *data, const std::string *filename) {
	struct soap strsoap;
	_genmech__mech *mech;	

	soap_init(&strsoap);
	soap_begin(&strsoap);
	std::ifstream mech_ifs;
	mech_ifs.open(filename->c_str(), std::ifstream::in);
	if (mech_ifs.fail()) {
		std::cout << "ERROR: Could not open file " << filename << std::endl;
		return 1;
	}
	strsoap.socket = -1; // Force inputstream usage.
	strsoap.is = &mech_ifs;	
	
	soap_begin_recv(&strsoap);
	if (!soap_get__genmech__mech(&strsoap, mech, "mech", NULL)) {
		std::cout << "Error reading mech from " << filename << std::endl;
		return 2;
	}
	soap_end_recv(&strsoap);

	int result = convertXmlDataToMechData(data, mech);
	if (result != 0) {
		return 100 + result;
	}

	soap_destroy(&strsoap);
	soap_end(&strsoap);
	soap_done(&strsoap);
	
	return 0;
}
} // namespace mechreader
} // namespace ap
