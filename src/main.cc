#include "photo_directory.hpp"
//#include <yuni/core/logs.h>


int main(int argc, char* const argv[])
{
	// To avoid compilation warnings
	(void) argc;
	(void) argv;

	SgPhoto::PhotoDirectory photoDirectory("/home/sebastien/Multimedia/Photos");

	//typedef Yuni::Logs::Logger<> LoggingFacility;
	//LoggingFacility logs;

	//logs.notice() << "HERE";

	auto directories = photoDirectory.tree();

	for (auto it = directories.cbegin(), end = directories.cend(); it != end; ++it)
	{
		std::cout << "Date = " << it->first << '\n';

		for (auto dir = it->second.cbegin(), end = it->second.cend(); dir != end; ++dir)
			std::cout << '\t' << *dir << '\n';
	}

	return 0;
}
