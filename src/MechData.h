#ifndef AP_MECHDATA_H
#define AP_MECHDATA_H

#include <OgreVector3.h>

#include "types.h"
#include "constants.h"
#include "net/NetObject.h"

#include <string>
#include <vector>

namespace ap {

    class MechDataMesh;

class MechData : public ap::net::NetObject
{
 public:
    MechData();
    ~MechData() {};

    // NetObject interface
    net::NetObject *create(uint32 id) const;
    uint8 getObjectType() const;

    //NetObject serialization
    int serialize(uint8 *buffer, int start, int buflength) const;
    int unserialize(uint8 *buffer, int start);

    int getTurnRate() const { return maxTurnRate; }
    int getMaxForwardAcceleration() const { return maxForwardAcceleration; }
    int getMaxBackwardAcceleration() const { return maxBackwardAcceleration; }
    int getMaxTorsoAngle() const { return maxTorsoAngle; }
    int getMaxSpeed() const { return maxSpeed; }
    std::string getName() const { return name; }
    std::string getTorsoMesh() const { return torsoMesh; }
    std::string getLegsMesh() const { return legsMesh; }

    void setTorsoMesh(const std::string &mesh) { torsoMesh = mesh; }
    void setLegsMesh(const std::string &mesh) { legsMesh = mesh; }
    void setTurnRate(int rate) { maxTurnRate = rate; }
    void setMaxForwardAcceleration(int acceleration) { maxForwardAcceleration = acceleration; }
    void setMaxBackwardAcceleration(int acceleration) { maxBackwardAcceleration = acceleration; }
    void setMaxTorsoAngle(int angle) { maxTorsoAngle = angle; }
    void setMaxSpeed(int speed) { maxSpeed = speed; }
    void setName(std::string name) { this->name = name; }


 private:
    std::string name;
    int maxTurnRate;
    int maxForwardAcceleration;
    int maxBackwardAcceleration;
    int maxTorsoAngle;
    int maxSpeed;

    std::string torsoMesh;
    std::string legsMesh;

    std::vector<MechDataMesh> meshes;
};

class MechDataMesh : public ap::net::Serializable
    {
    public:
	int serialize(uint8 buffer[], int start, int buflength) const;
	int unserialize(uint8 buffer[], int start);

	std::string getPartName() const { return partName; }
	std::string getParentName() const { return parentName; }
	std::string getFileName() const { return fileName; }
	Ogre::Vector3 getTranslation() const { return translation; }

    private:
	std::string partName;
	std::string parentName;
	std::string fileName;
	Ogre::Vector3 translation;
    };

} // namespace ap

#endif
