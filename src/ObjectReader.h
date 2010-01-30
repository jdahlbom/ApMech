#ifndef AP_OBJECTREADER_H
#define AP_OBJECTREADER_H

#include <string>

#include <expat.h>

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

namespace ap {

class MechReader {

public:
    MechReader (const std::string *filename) :
        turnRate(0), maxForwardAcceleration(0),
        maxBackwardAcceleration(0), maxTorsoAngle(0),
        maxSpeed(0), filename(filename),
        currentDataBuffer(""), torsoMesh("FullMech.mesh"),
	    legsMesh("CrudeMech.mesh") {}
    ~MechReader (void) {}

    bool parseFile();

    int getTurnRate() { return turnRate; }
    int getMaxForwardAcceleration() { return maxForwardAcceleration; }
    int getMaxBackwardAcceleration() { return maxBackwardAcceleration; }
    int getMaxTorsoAngle() { return maxTorsoAngle; }
    int getMaxSpeed() { return maxSpeed; }
    std::string getName() const { return name; }
    std::string getTorsoMesh() const {return torsoMesh;}
    std::string getLegsMesh() const {return legsMesh;}

    void setTurnRate(int rate) { turnRate = rate; }
    void setMaxForwardAcceleration(int acceleration) { maxForwardAcceleration = acceleration; }
    void setMaxBackwardAcceleration(int acceleration) { maxBackwardAcceleration = acceleration; }
    void setMaxTorsoAngle(int angle) { this->maxTorsoAngle = angle; }
    void setMaxSpeed(int speed) { this->maxSpeed = speed; }
    void setName(std::string name) { this->name = name; }
    void setTorsoMesh(std::string mesh) { this->torsoMesh = mesh; }
    void setLegsMesh(std::string mesh) { this->legsMesh = mesh; }

    bool addData(char *buf, int len);
    void resetData() { currentDataBuffer = ""; }
    std::string getData() { return currentDataBuffer; };

private:

    int turnRate;
    int maxForwardAcceleration;
    int maxBackwardAcceleration;
    int maxTorsoAngle;
    int maxSpeed;

    const std::string *filename;
    
    std::string currentDataBuffer;
    std::string name;

    std::string torsoMesh;
    std::string legsMesh;
};

} // namespace ap
#endif
