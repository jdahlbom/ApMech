#ifndef AP_OBJECTDATAMODEL_H
#define AP_OBJECTDATAMODEL_H

#include <map>
#include <string>

#include "constants.h"
#include "types.h"

namespace ap{

  class ObjectDataModel {
  public:
    ObjectDataModel();
    ~ObjectDataModel() {}

    // TODO: Should not take NetObject:ey objectType,
    //       instead some scheme of specific object type identification
    //       should be hatched. Good enough for this proto, though.
    std::string getMeshFilename(ap::uint8 objectType);
    
  private:
    std::map<ap::uint8, std::string> fileMap;
  }; // class ObjectDataModel

} // namespace ap

#endif
