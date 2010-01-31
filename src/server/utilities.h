#ifndef AP_SERVERUTILS_H
#define AP_SERVERUTILS_H

#include <string>

#include "../MechData.h"
#include "../ObjectDataBase.h"

namespace ap {
namespace server {

    ap::MechData *createMechData(ap::MechDatabase *mechDB, std::string mechname);

}
}
#endif
