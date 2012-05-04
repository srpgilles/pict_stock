#include "extended_photo.hpp"


int main(int argc, char* argv[])
{
	LoggingFacility logs;

	PictStock::ExtendedPhoto myPhoto(logs, "/home/sebastien/Photo_C0050.JPG");
	myPhoto.printExifData(std::cout);

	return 0;
}
