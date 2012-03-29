#ifndef AP_OBJECTREADER_H
#define AP_OBJECTREADER_H

#include <string>
#include <vector>


namespace ap {

class MechData;
namespace mechreader {
	int readMech(ap::MechData *result, const std::string *filename);
} // namespace mechreader
} // namespace ap
#endif
