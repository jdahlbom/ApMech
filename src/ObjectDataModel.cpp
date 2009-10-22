#include "ObjectDataModel.h"

#include "constants.h"
#include "types.h"

namespace ap {

  ObjectDataModel::ObjectDataModel() :
    fileMap(std::map<ap::uint8, std::string>())
  {
    fileMap.insert(std::make_pair<ap::uint8, std::string>(ap::OBJECT_TYPE_MECH, "CrudeMech.mesh"));
    fileMap.insert(std::make_pair<ap::uint8, std::string>(ap::OBJECT_TYPE_PROJECTILE, "CrudeMissile.mesh"));
  }
  
  /**
   * Gets the file name for the mesh of the requested type.
   *
   * Returns an empty string if no such object type exists in the model.
   *
   * @param objectType  Type identifier of the object.
   * @return File name of the mesh for this object, or empty string if invalid type.
   */
  std::string ObjectDataModel::getMeshFilename(ap::uint8 objectType) {
    std::map<ap::uint8, std::string>::const_iterator it;
    it = fileMap.find(objectType);
    if ( fileMap.end() == it ) {
      return std::string("");
    }
    
    return it->second;  
  }
} // namespace ap
