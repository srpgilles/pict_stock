#include "photo_directory.hpp"
#include "extended_photo.hpp"
#include "sort_new_photos.hpp"

namespace
{
	/*!
	 * \brief read the parameter file and check the expected fields are there
	 *
	 * \param File Parameters file. It is expected to fill values inputFolder,
	 * outputFolder and logFile
	 * \param[out] parameters Parameters read

	 * \return True if the value swere all correctly read
	 */
	bool readParameterFile(LoggingFacility& logs, const YString& file,
			std::map<Yuni::CString<12, false>, YString>& parameters)
	{
		YString key, value;

		if (!Yuni::IO::File::ReadLineByLine(file, [&] (const YString& line)
		{
			line.extractKeyValue(key, value, false);
			parameters[key] = value;
		}))
		{
			logs.error() << "Unable to read file " << file;
			return false;
		}

		return true;
	}

} // namespace anonymous

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

	std::map<Yuni::CString<12, false>, YString> parameters;

	if (!readParameterFile(logs, "parameters.ini", parameters))
		exit(EXIT_FAILURE);

	auto end = parameters.cend();

	auto iter = parameters.find("inputFolder");
	if (iter == end)
	{
		logs.fatal() << "inputFolder not read in parameter file";
		exit(EXIT_FAILURE);
	}
	YString inputFolder(iter->second);

	iter = parameters.find("outputFolder");
	if (iter == end)
	{
		logs.fatal() << "outputFolder not read in parameter file";
		exit(EXIT_FAILURE);
	}
	YString outputFolder(iter->second);

	iter = parameters.find("logFile");
	if (iter == end)
	{
		logs.fatal() << "logFile not read in parameter file";
		exit(EXIT_FAILURE);
	}
	YString logFile(iter->second);


	SgPhoto::PhotoDirectory photoDirectory(logs, outputFolder);

	SgPhoto::SortNewPhotos foo(logs, inputFolder, photoDirectory, logFile, true);

	if (!foo.proceed())
		return EXIT_FAILURE;

	logs.notice() << "OK";

	return 0;
}
