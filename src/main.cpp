#include "sort_new_photos/sort_new_photos.hpp"
#include <yuni/core/getopt.h>
#include "tools/read_parameter_file.hpp"

using namespace Yuni;


int main(int argc, char* argv[])
{
	LoggingFacility logs;

	bool doAskModifyDate(false);

	GetOpt::Parser parser;
	String parameterFile;
	parser.addParagraph("\nUsual option(s):\n");
	parser.add(parameterFile, ' ', "parameter_file", "Parameters file. If not precised "
		"parameters.ini in current folder is attempted.");
	parser.addParagraph("\nVery seldom and specific option(s):\n");
	AnyString description("If this flag is present, ask whenever a new folder is scanner for"
		" new photos whether a date is to be manually entered; if so the exif of the pictures "
		" inside that directory will be modified if the date doesn't match the date already there. "
		"This is useful for instance when applying the program to photos which exif date are "
		"uncertain but date are known are known otherwise.");
	parser.addFlag(doAskModifyDate, ' ', "do_ask_modify_date", description);
	parser.addParagraph("\nHelp:\n");

	if (!parser(argc, argv))
		exit(EXIT_FAILURE);

	// Default values
	if (parameterFile.empty())
		parameterFile = "parameters.ini";

	try
	{
		const GenericTools::ReadParameterFile parameters(logs, parameterFile);
		//PictStock::ExtendedPhoto(logs, "C:\\Users\\Sebastien\\Projets_logiciels\\Input\\2010-11-04\\023.JPG");
		PictStock::PhotoDirectory photoDirectory(logs, parameters["outputFolder"], parameters["pathFormat"]);
		PictStock::SortNewPhotos sortNewPhotos(logs, parameters["inputFolder"], photoDirectory,
			parameters["logFile"], doAskModifyDate);

		if (!sortNewPhotos.proceed())
			return EXIT_FAILURE; 
	}
	catch(const std::exception& e)
	{
		std::cout << "EXCEPTION: " << e.what() << '\n';
		exit(EXIT_FAILURE);
	}

	return 0;
}
