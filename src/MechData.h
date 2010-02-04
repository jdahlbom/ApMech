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
class MechDataVisitor;

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

    void setTurnRate(int rate) { maxTurnRate = rate; }
    void setMaxForwardAcceleration(int acceleration) { maxForwardAcceleration = acceleration; }
    void setMaxBackwardAcceleration(int acceleration) { maxBackwardAcceleration = acceleration; }
    void setMaxTorsoAngle(int angle) { maxTorsoAngle = angle; }
    void setMaxSpeed(int speed) { maxSpeed = speed; }
    void setName(std::string name) { this->name = name; }

    void accept(MechDataVisitor &visitor) const;
    void addMesh(const MechDataMesh &mesh);

 private:
    std::string name;
    int maxTurnRate;
    int maxForwardAcceleration;
    int maxBackwardAcceleration;
    int maxTorsoAngle;
    int maxSpeed;

    std::vector<MechDataMesh> meshes;
};

class MechDataMesh : public ap::net::Serializable
    {
    public:
	int serialize(uint8 buffer[], int start, int buflength) const;
	int unserialize(uint8 buffer[], int start);

	MechDataMesh();
	MechDataMesh(std::string name, 
		     std::string parent, 
		     std::string file, 
		     Ogre::Vector3 translation);

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

/**
 * Accessor interface for meshes-vector.
 * Implementing class is passed as parameter to accept-method
 * which performs the actual visitation functionality.
 * @see Visitor pattern
 */
class MechDataVisitor {
public:
    /**
     * @return True, if the visitor wishes to continue the visit.
     */
    virtual bool mdv_visit(const MechDataMesh &)=0;
};

} // namespace ap

#endif
