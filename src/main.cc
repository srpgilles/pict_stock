#include "photo_directory.hpp"
#include "extended_photo.hpp"
#include "sort_new_photos.hpp"


int main(int argc, char* const argv[])
{
	// To avoid compilation warnings
	(void) argc;
	(void) argv;

	LoggingFacility logs;
	//SgPhoto::PhotoDirectory photoDirectory(logs, "/home/sebastien/Multimedia/Photos");

	SgPhoto::ExtendedPhoto foo(logs, "/home/sebastien/Multimedia/Photos/2011/M02/J09/Photo_0736_DJ679_CSG.JPG");

	YString name;
	foo.newNameWithoutExtension(name);

	logs.notice() << name;

	SgPhoto::ExtendedPhoto bar(logs, "/home/sebastien/Multimedia/Photos/2011/M11/J27__Anniversaire_Sebastien/Photo_1414_DJ970_CSG.JPG");



	//SgPhoto::SortNewPhotos(logs, "/home/sebastien/Multimedia/Photos/A_classer", photoDirectory);

	return 0;
}
