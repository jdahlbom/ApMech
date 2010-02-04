#include "ObjectReader.h"
#include <stdlib.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <OgreVector3.h>

#include "MechData.h"

namespace ap {

void readAttributes(std::map<std::string, std::string> &attributes, const XML_Char **atts);

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

    mech->nodePath.push_back(std::string(name));
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

	return;
    }

    std::string path[] = {"mech", "media", "mesh"};
    if (mech->isPath(path, 3))
      {
	  std::map<std::string, std::string> attributes =
	      std::map<std::string, std::string>();
	  readAttributes(attributes, atts);

	  std::map<std::string, std::string>::const_iterator it;
	  it = attributes.find("type");
	  std::string type(it->second);
	  it = attributes.find("file");
	  std::string file(it->second);
	  it = attributes.find("translation");
	  std::string translation(it->second);
	  std::string parent("");
	  if (attributes.find("parent") != attributes.end())
	      parent = (attributes.find("parent"))->second;
	  
	  // TODO: parse translation here.
	  Ogre::Vector3 transl(0.0f, 10.0f, 0.0f);
	  MechDataMesh mesh(type, parent, file, transl);
	  mech->getMechData()->addMesh(mesh);
      }
    
    return;
}

void readAttributes(std::map<std::string, std::string> &attributes, const XML_Char **atts)
{
    attributes.clear();
    for ( int i=0; atts[i]; i+=2) {
	std::cout << "Param: " << atts[i] << ", Value: " << atts[i+1] << std::endl;
	std::string type(atts[i]);
	std::string value(atts[i+1]);
	attributes.insert(std::make_pair<std::string, std::string>(type, value));
    }
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

    mech->nodePath.pop_back();
    // everything is now ready, reset the buffer
    mech->resetData();

    return;
}


MechReader::MechReader (const std::string *filename) :
    nodePath(std::vector<std::string>()),
    data(NULL),
    filename(filename),
    currentDataBuffer("")
{
    data = new MechData();
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
        printf("Cannot open file [%s]\n", filename->c_str());
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

bool MechReader::isPath(std::string *path, size_t pathLength) const
{
#if 0
    printf("Requested path: ");
    for (size_t i=0; i<pathLength; ++i) {
        printf("%s / ", path[i].c_str());
    }
    printf("\n");
    printf("nodePath contains: ");
    std::vector<std::string>::const_iterator debug_itr = nodePath.begin();
    for (; debug_itr != nodePath.end();) {
      std::string a = static_cast<std::string>(*debug_itr);
      printf("%s / ", a.c_str());
      ++debug_itr;
    }
    printf("\n");
#endif

    if (pathLength != nodePath.size())
      return false;

    std::vector<std::string>::const_iterator itr = nodePath.begin();
    size_t i = 0;
    for (; itr!=nodePath.end() && i<pathLength;) {
        std::string pathElement = (std::string) *itr;
        if (path[i].compare(pathElement) != 0) // if given path is not equal! 
	{
	    return false;
        }
	++i;
	++itr;
    }
    return true;
}

} // namespace
