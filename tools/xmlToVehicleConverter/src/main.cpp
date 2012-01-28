#include <iostream>
#include <vector>

#include "../generated/generatedStub.h"
#include "../generated/genmech.nsmap"

using std::string;
using std::vector;

struct arguments {
	string sourceFile;
	string sourcePath;
	string targetFile;
};

void printHelp();

int main(int argc, char **argv) {
	const char *s_HELP = "-h";
	const char *l_HELP = "--help";
	const char *s_SOURCEFILE = "-f";
	const char *l_SOURCEFILE = "--sourceFile";
	const char *s_SOURCEPATH = "-p";
	const char *l_SOURCEPATH = "--sourcePath";
	const char *s_TARGET = "-t";
	const char *l_TARGET = "--targetFile";

	// Help!
	if (argc == 2 && (strcmp(argv[1], "-h")==0 || strcmp(argv[1], "--help")==0)) {
		printHelp();
		return 0;
	} else if (argc == 7) {
	

	} else {
		std::cout << "Invalid arguments, run \"createmech\" " << s_HELP << "\" for help" << std::endl;
		return 1;
	}

	return 0;
}

void printHelp() {
	using std::cout;
	using std::endl;
	cout << "\nUsage:  createmech -f sourcefilename -p path/to/sourcefile -t targetfilename" << endl;
	cout << "\nThis utility program takes a blender->ogre imported mesh.xml and skeleton.xml" << endl;
	cout << "and creates a corresponding blank new mech description XML file with given mesh" << endl;
	cout << "file and generated equipment slot defintions from the skeleton bones." << endl;

	cout << "\nOPTIONS:" << endl;
	cout << "-h, --help\t\tPrints this help" << endl;
	cout << "-f, --sourceFile\tSource file name without path" << endl;
	cout << "-p, --sourcePath\tSource file path containing both mesh and skeleton files" << endl;
	cout << "-t, --targetFile\tFile name for the newly created mech xml file" << endl << endl;
}
