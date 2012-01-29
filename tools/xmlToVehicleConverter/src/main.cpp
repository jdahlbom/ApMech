#include <iostream>
#include <fstream>
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
int doWork(char *sourceFile, char *sourcePath, char *targetFile);

int main(int argc, char **argv) {
	const char *s_HELP = "-h";
	const char *l_HELP = "--help";
	char *sourceFile;
	char *sourcePath;
	char *targetFile;

	// Help!
	if (argc == 2 && (strcmp(argv[1], s_HELP)==0 || strcmp(argv[1], l_HELP)==0)) {
		printHelp();
		return 0;
	} else if (argc == 4) {
		sourceFile = argv[1];
		sourcePath = argv[2];
		targetFile = argv[3];
		return doWork(sourceFile, sourcePath, targetFile);
	} else {
		std::cout << "Invalid arguments, run \"createmech\" " << s_HELP << "\" for help" << std::endl;
		return 1;
	}

	return 0;
}

void printHelp() {
	using std::cout;
	using std::endl;
	cout << "\nUsage:  createmech source_mesh_name path/to/sourcefile targetfilename" << endl;
	cout << "\nThis utility program takes a blender->ogre imported mesh.xml and skeleton.xml" << endl;
	cout << "and creates a corresponding blank new mech description XML file with given mesh" << endl;
	cout << "file and generated equipment slot defintions from the skeleton bones." << endl;
	cout << "Source mesh name should exclude the file extensions: MyFile.mesh.xml --> MyFile" << endl;

	cout << "\nOPTIONS:" << endl;
	cout << "-h, --help\t\tPrints this help" << endl;
}

int doWork(char *sourceFile, char *sourcePath, char *targetFile) {
	struct soap strsoap;
	_genmesh__mesh mesh;
	_genskel__skeleton skeleton;
	_genmech__mech mech;

	soap_init(&strsoap);
	soap_begin(&strsoap);

	std::ifstream mesh_ifs;
	char *inputFile;
	inputFile = (char *) malloc(strlen(sourceFile)+strlen(sourcePath)+strlen(".mesh.xml")+2);
	strcpy(inputFile, sourcePath);
	strcat(inputFile, "/");
	strcat(inputFile, sourceFile);
	strcat(inputFile, ".mesh.xml");
	mesh_ifs.open(inputFile, std::ifstream::in);
	if (mesh_ifs.fail()) {
		std::cout << "Could not open file " << inputFile << std::endl;
		free(inputFile);
		return 1;
	}
	strsoap.socket = -1;
	strsoap.is = &mesh_ifs;

	// Read MESH from file.
	soap_begin_recv(&strsoap);
	if (0 == soap_get__genmesh__mesh(&strsoap, &mesh, "mesh", NULL)) {
		std::cout << "Unable to read mesh from file!" << std::endl;
		free(inputFile);
		return 2;
	}
	soap_end_recv(&strsoap);
	free(inputFile);

	if (mesh.genmesh__skeletonlink == 0) {
		std::cout << "Given mesh does not have a skeleton, aborting!" << std::endl;
		return 3;
	}

	std::string skeletonName(mesh.genmesh__skeletonlink->name.c_str());
	// Close mesh file, delete temporaries.
	mesh_ifs.close();
	soap_destroy(&strsoap);
	soap_end(&strsoap);
	soap_done(&strsoap);

	// Initialize context for reading from SKELETON
	soap_init(&strsoap);
	soap_begin(&strsoap);

	std::ifstream skeleton_ifs;
	char *skeletonFile = (char *) malloc(strlen(sourcePath) + 
		strlen(".xml") + strlen(skeletonName.c_str()) + 2);
	strcpy(skeletonFile, sourcePath);
	strcat(skeletonFile, "/");
	strcat(skeletonFile, skeletonName.c_str());
	strcat(skeletonFile, ".xml\0");
	skeleton_ifs.open(skeletonFile, std::ifstream::in);
	if (skeleton_ifs.fail()) {
		std::cout << "Failed to open skeleton file " << skeletonFile << std::endl;
		free(skeletonFile);
		return 4;
	}

	strsoap.socket = -1;
	strsoap.is = &skeleton_ifs;

	// Reading SKELETON from file.
	soap_begin_recv(&strsoap);
	if (!soap_get__genskel__skeleton(&strsoap, &skeleton, "skeleton", NULL)) {
		std::cout << "Error reading skeleton from " << skeletonFile << std::endl;
		free(skeletonFile);
		return 5;
	}

	std::cout << "Read skeleton OK!" << std::endl;
	free(skeletonFile);
	return 0;
}
