

#include <yuni/core/getopt.h>
#include "tools/read_parameter_file.hpp"
#include "extended_photo/date.hpp"
#include "photo_directory/private/path_format_helpers.hpp"


using namespace Yuni;

namespace
{
	static const YString expression =
		YString('^')
			<< '(' << PictStock::Private::Year::Regex() << ")"
			<< '-' // separator
			<< '(' << PictStock::Private::Month::Regex() << ")"
			<< '-' // separator
			<< '(' << PictStock::Private::Day::Regex() << ")"
			<< '$';

	/*!
	** \brief Regular expression for date formatting
	**
	** Basically format is YYYY:MM:DD HH:mm:SS
	*/
	//static const regexNS::regex RegexDateFormatting(expression.c_str());
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

		const GenericTools::ReadParameterFile parameters(logs, parameterFile, keys);

		PictStock::Private::PathFormatHelper helper(logs, "%y-%m-%d");
		PictStock::PathInformations infos(logs);

		auto strBeginDate = parameters["endDate"];
		strBeginDate.trim();
		if (strBeginDate == "none")
		{
			logs.notice("Take 0");
		}
		else
		{
			logs.notice() << helper.isOk(parameters["beginDate"], infos);
			infos.print(std::cout);
		}
	}
	catch(const std::exception& e)
	{
		std::cout << "EXCEPTION: " << e.what() << '\n';
		exit(EXIT_FAILURE);
	}

	return 0;
}
