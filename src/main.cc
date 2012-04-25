#include "photo_directory.hpp"
#include "extended_photo.hpp"
#include "sort_new_photos.hpp"


int main(int argc, char* const argv[])
{
	// To avoid compilation warnings
	(void) argc;
	(void) argv;

	LoggingFacility logs;
	SgPhoto::PhotoDirectory photoDirectory(logs, "/tmp/devPhotos");


	SgPhoto::SortNewPhotos foo(logs, "/home/sebastien/Multimedia/Photos/A_classer", photoDirectory, true);
	if (!foo.proceed())
		return EXIT_FAILURE;

	logs.notice() << "OK";

	return 0;
}
