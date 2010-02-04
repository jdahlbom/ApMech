#include "MechData.h"
#include "net/serializer.h"

namespace ap {

/**//* This mystical part here pushes exactly ONE object of this type to the prototype map, */
/**//* in order to easily unserialize stuff. Idea gotten from C++ FAQ Lite by Marshall Cline*/
/**/class MechDataInject {
/**/  public:
/**/    MechDataInject() {
/**/      ap::net::netobjectprototypes().insert(
/**/        std::make_pair(ap::OBJECT_TYPE_MECHDATA, new MechData()));
/**/    }
/**/};
/**/static MechDataInject __mechdatainject;


    MechData::MechData() : 
	name(""), 
	maxTurnRate(0), 
	maxForwardAcceleration(0), 
	maxBackwardAcceleration(0),
	meshes(std::vector<MechDataMesh>()) {}

net::NetObject * MechData::create(uint32 id) const
{
    ap::net::NetObject *pNetObject = new MechData();
    pNetObject->id = id;
    return pNetObject;
}

int MechData::serialize(uint8 buffer[], int start, int buflength) const
{
    int length = 0;
    length += ap::net::serialize(name, buffer, start+length, buflength-length);
    length += ap::net::serialize(maxForwardAcceleration, buffer, start+length, buflength-length);
    length += ap::net::serialize(maxBackwardAcceleration, buffer, start+length, buflength-length);
    length += ap::net::serialize(maxTurnRate, buffer, start+length, buflength-length);
    length += ap::net::serialize(maxTorsoAngle, buffer, start+length, buflength-length);
    length += ap::net::serialize(maxSpeed, buffer, start+length, buflength-length);

    ap::uint8 numMeshes = meshes.size();
    length += ap::net::serialize(numMeshes, buffer, start+length, buflength-length);
    std::vector<MechDataMesh>::const_iterator it;
    for (it = meshes.begin(); it!=meshes.end(); ++it) {
	std::cout << "Serializing mesh [" << (*it).getPartName() << "]=[" << (*it).getFileName() <<"]" << std::endl;
	length += (*it).serialize(buffer, start+length, buflength-length);
    }

    return length;
}

int MechData::unserialize(uint8 buffer[], int start)
{
    int length = 0;
    length += ap::net::unserialize(name, buffer, start+length);
    length += ap::net::unserialize(maxForwardAcceleration, buffer, start+length);
    length += ap::net::unserialize(maxBackwardAcceleration, buffer, start+length);
    length += ap::net::unserialize(maxTurnRate, buffer, start+length);
    length += ap::net::unserialize(maxTorsoAngle, buffer, start+length);
    length += ap::net::unserialize(maxSpeed, buffer, start+length);
    
    uint8 numberOfMeshes = 0;
    length += ap::net::unserialize(numberOfMeshes, buffer, start+length);
    meshes.clear();
    for (int i=0; i<numberOfMeshes; ++i) {
	MechDataMesh newMesh = MechDataMesh();
	length += newMesh.unserialize(buffer, start+length);
	meshes.push_back(newMesh);
	std::cout << "Serializing mesh [" << newMesh.getPartName() << "]=[" << newMesh.getFileName() <<"]" << std::endl;
    }

    return length;
}

ap::uint8 MechData::getObjectType() const {
    return ap::OBJECT_TYPE_MECHDATA;
}

/**
 * Accepts a visitor to iterate through the mesh list.
 * @see Visitor pattern
 */
void MechData::accept(MechDataVisitor &visitor) const {
    std::vector<MechDataMesh>::const_iterator it;
    
    it = meshes.begin();
    while (it!=meshes.end()) {
	bool shouldContinue = visitor.mdv_visit(*it);
	if (!shouldContinue)
	    break;
	++it;
    }
}

void MechData::addMesh(const MechDataMesh &mesh) {
    meshes.push_back(mesh);
}

    MechDataMesh::MechDataMesh() {} // NOTE: Only use for unserialization purposes!

    MechDataMesh::MechDataMesh(std::string name, 
			       std::string parent, 
			       std::string file, 
			       Ogre::Vector3 _translation) :
	partName(name),
	parentName(parent),
	fileName(file),
	translation(_translation) {}

    int MechDataMesh::serialize(uint8 buffer[], int start, int buflength) const
    {
	int length = 0;
	length += ap::net::serialize(partName, buffer, start+length, buflength-length);
	length += ap::net::serialize(parentName, buffer, start+length, buflength-length);
	length += ap::net::serialize(fileName, buffer, start+length, buflength-length);
	length += ap::net::serialize(translation, buffer, start+length, buflength-length);
	return length;
    }

    int MechDataMesh::unserialize(uint8 buffer[], int start)
    {
	int length;
	length += ap::net::unserialize(partName, buffer, start+length);
	length += ap::net::unserialize(parentName, buffer, start+length);
	length += ap::net::unserialize(fileName, buffer, start+length);
	length += ap::net::unserialize(translation, buffer, start+length);
	return length;
    }
} // namespace ap
