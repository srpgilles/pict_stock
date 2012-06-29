#include <yuni/core/getopt.h>
#include "tools/read_parameter_file.hpp"
#include "tools/exceptions.hpp"
#include "extended_photo/date.hpp"
#include "photo_directory/private/path_format_helpers.hpp"
#include "pict_frame/pict_frame.hpp"
#include <numeric>

using namespace Yuni;

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
		time_t& out,
		const PictStock::Private::PathFormatHelper& helper,
		const GenericTools::ReadParameterFile& parameters,
		const YString& dateAsString)
	{
		const auto& parameterValue = parameters[dateAsString];

		if (parameterValue != "none")
		{
			PictStock::PathInformations infos(logs);

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
	LoggingFacility logs;

	GetOpt::Parser parser;
	String parameterFile;
	parser.addParagraph("\nUsual option(s):\n");
	parser.add(parameterFile, ' ', "parameter_file", "Parameters file. If not precised "
		"parameters_pict_frame.ini in current folder is attempted.");
	String strBeginDate, strEndDate;
	parser.add(strBeginDate, ' ', "begin_date", "Photos older than this date aren't "
		"taken into account. Leave it empty if no such restriction; otherwise format is"
		"{year}-{month}-{day}");
	parser.add(strEndDate, ' ', "end_date", "Photos newer than this date aren't "
		"taken into account. Leave it empty if no such restriction; otherwise format is"
		"{year}-{month}-{day}");

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

		PictStock::Private::PathFormatHelper helper(logs, "%y-%m-%d");

		time_t beginDate(0);
		time_t endDate(std::numeric_limits<time_t>::max());
		determineTimeLimits(logs, beginDate, helper, parameters, "beginDate");
		determineTimeLimits(logs, endDate, helper, parameters, "endDate");

		PictStock::ReadDate::Mode mode;
		{
			if (parameters["readDateMode"] == "safe")
				mode = PictStock::ReadDate::safe;
			else if (parameters["readDateMode"] == "fast")
				mode = PictStock::ReadDate::fast;
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

		PictStock::PictFrame(logs, parameters["pathFormat"], parameters["inputFolder"],
			parameters["outputFolder"], nbPhotos, beginDate, endDate,
			mode, isChronological);

	}
	catch(const std::exception& e)
	{
		std::cout << "EXCEPTION: " << e.what() << '\n';
		exit(EXIT_FAILURE);
	}

	return 0;
}
