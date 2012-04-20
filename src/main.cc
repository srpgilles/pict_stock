#include "photo_directory.hpp"
#include "extended_photo.hpp"


int main(int argc, char* const argv[])
{
	// To avoid compilation warnings
	(void) argc;
	(void) argv;

	LoggingFacility logs;
	SgPhoto::PhotoDirectory photoDirectory(logs, "/home/sebastien/Multimedia/Photos");
	
	SgPhoto::ExtendedPhoto myPhoto(logs, "/home/sebastien/Multimedia/Photos/2012/M04/J16/Photo_1915_DJ1111_CSG.JPG");
	
	logs.notice() << myPhoto.problem();
	logs.notice() << myPhoto.date();
	logs.notice() << myPhoto.newName();


	return 0;
}
