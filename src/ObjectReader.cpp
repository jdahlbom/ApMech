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
    mech->resetData();

#if 0
    printf("> startHandler for '%s'\n", name);
#endif

    if (strcmp(name, "torso") == 0) {

        if (atts) {
            int i;

            for (i = 0; atts[i]; i += 2) {
#if 0
                printf("attribute '%s', value '%s'\n", atts[i], atts[i+1]);
#endif
                if (strcmp(atts[i], "turningRadius") == 0 && atts[i+1] != NULL) {
                    /* TODO: error handling */
                    int angle = strtol(atts[i+1], NULL, 10);
                    mech->setMaxTorsoAngle(angle);
                }
            }
        }
    }

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
    else if (strcmp(name, "maxSpeed") == 0) {
        int speed = strtol(mech->getData().c_str(), NULL, 10);
        mech->setMaxSpeed(speed);
    }
    else if (strcmp(name, "name") == 0) {
        std::string name = mech->getData();
        mech->setName(name);
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
    
    FILE *fp;
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

    if((fp = fopen(filename->c_str(), "r")) == NULL) {
        printf("Cannot open file: %s.\n", filename->c_str());
        return false;
    }

    do {
        readBytes = fread(buf, 1, 255, fp);

        if (readBytes < 0) {
            printf("Read error.\n");
            break;
        }
        else if (feof(fp)) {
            wasLast = 1;
        }

#if 0
        printf("read %i bytes, %s\n", readBytes, wasLast ? "was last" : "wasn't last");
#endif

        XML_Parse(parser, buf, readBytes, wasLast);
        
    } while (!wasLast);

    fclose(fp);

    XML_ParserFree(parser);

    return true;
}

}
