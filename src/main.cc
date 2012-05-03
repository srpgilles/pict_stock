#include "sort_new_photos.hpp"
#include <yuni/core/getopt.h>

using namespace Yuni;

namespace
{


	/*!
	 * \brief Read the parameter file and check the expected fields are there
	 *
	 * \param File Parameters file. It is expected to fill values inputFolder,
	 * outputFolder and logFile
	 * \param mandatoryKeys Keys that are expected in the parameters file
	 * \param[out] parameters Parameters read

	 * \return True if the values were all correctly read
	 */
	class ReadParameterFile
	{
	public:

		// Type of keys in the parameters file
		typedef CString<12, false> KeyString;

		/*!
		 ** Constructor
		 **
		 ** It read the parameter files, load into the class the values that were expected
		 ** and finally check all expected values were actually filled
		 */
		ReadParameterFile(LoggingFacility& logs, const YString& file);

		//! Access to parameters; return false if one is not known
		const String& operator[](const KeyString& key) const;


	public:

		//! Logs
		mutable LoggingFacility& logs;


	private:

		//! Check #pParameters is correctly filled
		bool checkParameters() const;



	private:

		//! Parameter file
		const String& pFile;

		/*!
		** \brief Map containing all relevant values read and their associated values
		**
		** Important: before call to #proceed only the keyx are defined; values are left empty
		*/
		std::map<KeyString, String> pParameters;


	};


	ReadParameterFile::ReadParameterFile(LoggingFacility& logs, const YString& file)
		: logs(logs),
		  pFile(file),
		  pParameters({{"inputFolder",""}, {"outputFolder",""}, {"logFile",""}})
	{

		// Assign values from the parameter file
		String key, value;
		auto end = pParameters.end();

		if (!IO::File::ReadLineByLine(pFile, [&] (const String& line)
		{
			line.extractKeyValue(key, value, false);

			auto it = pParameters.find(key);

			if (it == end)
			{
				// Check the key read belongs to the mandatory list given in constructor
				// If not, issue a warning
				logs.warning() << "Parameter " << key << " read in file not foreseen; "
					"it will be ignored";
			}
			else
			{
				value.trim();
				it->second = value;
			}
		}))
		{
			logs.fatal() << "Unable to read file " << pFile;
			throw std::exception();
		}

		if (!checkParameters())
			throw std::exception();
	}


	bool ReadParameterFile::checkParameters() const
	{
		for (auto it = pParameters.cbegin(), end = pParameters.cend(); it != end; ++it)
		{
			logs.checkpoint() << it->first << '\t' << it->second;

			if ((it->second).empty())
			{
				logs.error("Parameter ") << it->first << " not filled in parameters "
					"file " << pFile;
				return false;
			}
		}

		return true;
	}


	const String& ReadParameterFile::operator[](const KeyString& key) const
	{
		auto it = pParameters.find(key);
		assert(it != pParameters.end() && "Should be enforced in #proceed() method");
		return it->second;
	}



} // namespace anonymous




int main(int argc, char* argv[])
{
	LoggingFacility logs;


	bool doAskModifyDate;

	GetOpt::Parser parser;
	String parameterFile;
	parser.addParagraph("\nMandatory option(s):\n");
	parser.add(parameterFile, ' ', "parameter_file", "Parameters file");
	parser.addParagraph("\nVery seldom and specific option(s):\n");
	AnyString description("If this flag is present, ask whenever a new folder is scanner for"
		" new photos whether a date is to be manually entered; if so the exif of the pictures "
		" inside that directory will be modified if the date doesn't match the date already there. "
		"This is useful for instance when applying the program to photos which exif date are "
		"uncertain but date are known are known otherwise.");
	parser.addFlag(doAskModifyDate, ' ', "do_ask_modify_date", description);
	parser.addParagraph("\nHelp:\n");

	if (!parser(argc, argv))
	{
		logs.fatal("Command line parsing failed. Here is the help description of the program:");
		parser.helpUsage(argv[0]);
		exit(EXIT_FAILURE);
	}

	try
	{
		const ReadParameterFile parameters(logs, parameterFile);

		PictStock::PhotoDirectory photoDirectory(logs, parameters["outputFolder"]);
		PictStock::SortNewPhotos sortNewPhotos(logs, parameters["inputFolder"], photoDirectory,
			parameters["logFile"], doAskModifyDate);

		if (!sortNewPhotos.proceed())
			return EXIT_FAILURE;
	}
	catch(const std::exception& e)
	{
		exit(EXIT_FAILURE);
	}

	return 0;
}
