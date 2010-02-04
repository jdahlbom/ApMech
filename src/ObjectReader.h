#ifndef AP_OBJECTREADER_H
#define AP_OBJECTREADER_H

#include <string>

#include <expat.h>

#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <vector>

#include "MechData.h"

namespace ap {

class MechReader {

public:
    MechReader (const std::string *filename);
    ~MechReader (void) {}

    bool parseFile();

    int getTurnRate() { return data->getTurnRate(); }
    int getMaxForwardAcceleration() { return data->getMaxForwardAcceleration(); }
    int getMaxBackwardAcceleration() { return data->getMaxBackwardAcceleration(); }
    int getMaxTorsoAngle() { return data->getMaxTorsoAngle(); }
    int getMaxSpeed() { return data->getMaxSpeed(); }
    std::string getName() const { return data->getName(); }

    void setTurnRate(int rate) { data->setTurnRate(rate); }
    void setMaxForwardAcceleration(int acceleration) { data->setMaxForwardAcceleration(acceleration); }
    void setMaxBackwardAcceleration(int acceleration) { data->setMaxBackwardAcceleration(acceleration); }
    void setMaxTorsoAngle(int angle) { data->setMaxTorsoAngle(angle); }
    void setMaxSpeed(int speed) { data->setMaxSpeed(speed); }
    void setName(std::string name) { data->setName(name); }

    MechData* getMechData() { return data; }

    bool addData(char *buf, int len);
    void resetData() { currentDataBuffer = ""; }
    std::string getData() { return currentDataBuffer; };

    std::vector<std::string> nodePath;
    bool isPath(std::string *path, size_t pathLength) const;

private:
    MechData *data;
    const std::string *filename;
    std::string currentDataBuffer;
};

} // namespace ap
#endif
