#ifndef AP_OBJECTDATAMODEL_H
#define AP_OBJECTDATAMODEL_H

#include <map>
#include <string>

#include "constants.h"
#include "types.h"

namespace ap {

    class ObjectDataModel {
        public:
            static ObjectDataModel *getInstance();
            ~ObjectDataModel() {}

            // TODO: Should not take NetObject:ey objectType,
            //       instead some scheme of specific object type identification
            //       should be hatched. Good enough for this proto, though.
            std::string getMeshFilename(ap::uint8 objectType);
            
            float getMaxForwardAcceleration(ap::uint8 objectType);
            float getMaxBackwardAcceleration(ap::uint8 objectType);
            float getMaxTurningRate(ap::uint8 objectType);

        private:
            ObjectDataModel();
            std::map<ap::uint8, std::string> fileMap;
            static ObjectDataModel *instance;
    }; // class ObjectDataModel

} // namespace ap

#endif
