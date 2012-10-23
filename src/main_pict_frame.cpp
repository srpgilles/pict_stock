#include <yuni/core/getopt.h>
#include "tools/read_parameter_file.hpp"
#include "tools/exceptions.hpp"
#include "tools/sqlite_wrapper.hpp"
#include "extended_photo/date.hpp"
#include "database/cameras.hpp"
#include "photo_directory/private/path_format_helpers.hpp"
#include "pict_frame/pict_frame.hpp"
#include <QApplication>

using namespace Yuni;
using namespace PictStock;

namespace
{

	/*!
	** \brief Read the date string in parameter file and determine from it relevant time stamp
	**
	** \param[in, out] out Time stamp matching the date. If dateAsString is "none", input value
	** is left unchanged
	** @param helper PathFormatHelper object, to interpret the date read
	** @param parameters Parameters as read in the parameter file
	** @param dateAsString Key of the parameter looked at
	**/

	void determineTimeLimits(LoggingFacility& logs,
		const ExtendedPhoto::Cameras& cameras,
		time_t& out,
		const PhotoDirectory::Private::PathFormatHelper& helper,
		const GenericTools::ReadParameterFile& parameters,
		const YString& dateAsString)
	{
		const auto& parameterValue = parameters[dateAsString];

		if (parameterValue != "none")
		{
			ExtendedPhoto::PathInformations infos(logs, cameras);

			if (!helper.isOk(parameterValue, infos))
			{
				logs.fatal("Invalid ") << dateAsString << " in parameters file";
				exit(EXIT_FAILURE);
			}
			out = infos.date().timeStamp();
		}
	}


}// namespace anonymous


int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	LoggingFacility logs;

	GetOpt::Parser parser;
	String parameterFile;
	parser.addParagraph("\nUsual option(s):\n");
	parser.add(parameterFile, ' ', "parameter_file", "Parameters file. If not precised "
		"parameters_pict_frame.ini in current folder is attempted.");

	parser.addParagraph("\nHelp:\n");

	if (!parser(argc, argv))
		exit(EXIT_FAILURE);

	// Default values
	if (parameterFile.empty())
		parameterFile = "parameters_pict_frame.ini";

	try
	{
		typedef GenericTools::ReadParameterFile::KeyString KeyString;
		std::list<KeyString> keys;
		keys.push_back("inputFolder");
		keys.push_back("outputFolder");
		keys.push_back("pathFormat");
		keys.push_back("beginDate");
		keys.push_back("endDate");
		keys.push_back("nbPhotos");
		keys.push_back("readDateMode");
		keys.push_back("isChronological");

		const GenericTools::ReadParameterFile parameters(logs, parameterFile, keys);
		GenericTools::SqliteWrapper db("test.db3", SQLITE_OPEN_READWRITE);
		ExtendedPhoto::Cameras cameras(db);


		PhotoDirectory::Private::PathFormatHelper helper(logs, "%Y-%m-%d");

		time_t beginDate(0);
		time_t endDate(std::numeric_limits<int>::max()); // int because struct tm components are defined as such

		determineTimeLimits(logs, cameras, beginDate, helper, parameters, "beginDate");
		determineTimeLimits(logs, cameras, endDate, helper, parameters, "endDate");

		PictFrame::ReadDate::Mode mode;
		{
			if (parameters["readDateMode"] == "safe")
				mode = PictFrame::ReadDate::safe;
			else if (parameters["readDateMode"] == "fast")
				mode = PictFrame::ReadDate::fast;
			else
				throw GenericTools::Exception("Mode in parameters file must be "
					"either safe or fast");
		}

		bool isChronological;

		{
			if (parameters["isChronological"] == "true")
				isChronological = true;
			else if (parameters["isChronological"] == "false")
				isChronological = false;
			else
				throw GenericTools::Exception("isChronological in parameters "
					"file must be either true or false");
		}

		unsigned int nbPhotos;
		{
			if (!parameters["nbPhotos"].to(nbPhotos))
				throw GenericTools::Exception("nbPhotos must be a positive integer");
		}


		PictFrame::PictFrame(logs, cameras, parameters["pathFormat"], parameters["inputFolder"],
			parameters["outputFolder"], nbPhotos, beginDate, endDate,
			mode, isChronological);



	}
	catch(const std::exception& e)
	{
		std::cout << "EXCEPTION: " << e.what() << '\n';
		exit(EXIT_FAILURE);
	}

    return app.exec();
}
