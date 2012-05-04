#include "extended_photo.hpp"


int main(int argc, char* argv[])
{
	(void) argc;
	(void) argv;

	LoggingFacility logs;

	PictStock::ExtendedPhoto myPhoto(logs, "/home/sebastien/Photo_C0050.JPG");
	YString name;
	myPhoto.newNameWithoutExtension(name);
	logs.notice(myPhoto.date());
	logs.notice(myPhoto.time());
	logs.notice(name);

	return 0;
}
