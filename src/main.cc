#include "photo_directory.hpp"
#include "extended_photo.hpp"
#include "sort_new_photos.hpp"


int main(int argc, char* const argv[])
{
	// To avoid compilation warnings
	(void) argc;
	(void) argv;

	LoggingFacility logs;

//	SgPhoto::ExtendedPhoto bar(logs, "/tmp/devPhotos3/2011/M06/J30/Photo_2259_UNK.jpg");
//	YString name;
//	bar.newNameWithoutExtension(name);
//	logs.notice() << name;
////	bar.printExifData(std::cout);
//
//	return 0;

	SgPhoto::PhotoDirectory photoDirectory(logs, "/tmp/devPhotos4");

	SgPhoto::SortNewPhotos foo(logs, "/tmp/devPhotos3", photoDirectory, false);
	if (!foo.proceed())
		return EXIT_FAILURE;

	logs.notice() << "OK";

	return 0;
}
