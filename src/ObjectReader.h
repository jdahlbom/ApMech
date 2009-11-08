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
        maxBackwardAcceleration(0), filename(filename),
        currentDataBuffer("") {}
    ~MechReader (void) {};

    bool parseFile();

    int getTurnRate() { return turnRate; }
    int getMaxForwardAcceleration() { return maxForwardAcceleration; }
    int getMaxBackwardAcceleration() { return maxBackwardAcceleration; }
    std::string getName() const { return name; }
    
    void setTurnRate(int rate) { turnRate = rate; }
    void setMaxForwardAcceleration(int acceleration) { maxForwardAcceleration = acceleration; }
    void setMaxBackwardAcceleration(int acceleration) { maxBackwardAcceleration = acceleration; }
    void setName(std::string name) { this->name = name; }

    bool addData(char *buf, int len);
    void resetData() { currentDataBuffer = ""; }
    std::string getData() { return currentDataBuffer; };

private:

    int turnRate;
    int maxForwardAcceleration;
    int maxBackwardAcceleration;

    const std::string *filename;
    
    std::string currentDataBuffer;
    std::string name;

};

} // namespace ap
#endif
