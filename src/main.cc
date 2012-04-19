#include "photo_directory.hpp"



int main(int argc, char* const argv[])
{
	// To avoid compilation warnings
	(void) argc;
	(void) argv;

	LoggingFacility logs;
	SgPhoto::PhotoDirectory photoDirectory(logs, "/home/sebastien/Multimedia/Photos");

	//photoDirectory.print(std::cout);



	return 0;
}
