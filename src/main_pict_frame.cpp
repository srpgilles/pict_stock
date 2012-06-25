

#include <yuni/core/getopt.h>
#include "tools/read_parameter_file.hpp"
#include "extended_photo/date.hpp"


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
		const GenericTools::ReadParameterFile parameters(logs, parameterFile);

		//Yuni::DateTime::


	}
	catch(const std::exception& e)
	{
		std::cout << "EXCEPTION: " << e.what() << '\n';
		exit(EXIT_FAILURE);
	}

	return 0;
}
