#include "ObjectReader.h"
#include <stdlib.h>

namespace ap {

void characterDataHandler(void *userData, const XML_Char *s, int len) {
    
    MechReader *mech = (MechReader *) userData;

    mech->addData((char *) s, len);

#if 0
    char buf[len+1];

    if (len == 0)
        return;

    memcpy(buf, s, len);
    buf[len] = '\0';

    printf("> data: '%s'\n", buf);
#endif

    return;
}

void startHandler(void *userData, const XML_Char *name, const XML_Char **atts) {

    MechReader *mech = (MechReader *) userData;

#if 0
    printf("> startHandler for '%s'\n", name);
#endif

    return;
}

void endHandler(void *userData, const XML_Char *name) {
    
    MechReader *mech = (MechReader *) userData;
    
#if 0
    printf("> endHandler for '%s'\n", name);

    printf("%s\n", mech->getData().c_str());
#endif

    if (strcmp(name, "maxTurnRate") == 0) {
        int rate = strtol(mech->getData().c_str(), NULL, 10);
        mech->setTurnRate(rate);
    }
    else if (strcmp(name, "maxForwardAcceleration") == 0) {
        int rate = strtol(mech->getData().c_str(), NULL, 10);
        mech->setMaxForwardAcceleration(rate);
    }
    else if (strcmp(name, "maxBackwardAcceleration") == 0) {
        int rate = strtol(mech->getData().c_str(), NULL, 10);
        mech->setMaxBackwardAcceleration(rate);
    }

    // everything is now ready, reset the buffer
    mech->resetData();

    return;
}

bool MechReader::addData(char *buf, int len) {

    std::string tmp;

    tmp.assign(buf, len);
    currentDataBuffer += tmp;

    return true;
}

bool MechReader::parseFile() {
    
    int fd;
    char buf[256];
    ssize_t readBytes;
    int wasLast = 0;

    XML_Parser parser = XML_ParserCreate("UTF-8");
    if (parser == NULL) {
        printf("Failed to create parser.\n");
        return false;
    }

    XML_SetElementHandler(parser, startHandler, endHandler);
    XML_SetCharacterDataHandler(parser, characterDataHandler);
    XML_SetParamEntityParsing(parser, XML_PARAM_ENTITY_PARSING_ALWAYS);
    XML_SetUserData(parser, (void *) this);

    if((fd = open(filename->c_str(), O_RDONLY)) < 0) {
        printf("Cannot open file.\n");
        return false;
    }

    do {
        readBytes = read(fd, buf, 255);

        if (readBytes < 0) {
            printf("Read error.\n");
            break;
        }
        else if (readBytes < 255) {
            wasLast = 1;
        }

#if 0
        printf("read %i bytes, %s\n", readBytes, wasLast ? "was last" : "wasn't last");
#endif

        XML_Parse(parser, buf, readBytes, wasLast);
        
    } while (!wasLast);

    close(fd);

    XML_ParserFree(parser);

    return true;
}

}
