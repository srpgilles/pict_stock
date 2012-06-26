#include "read_parameter_file.hpp"

using namespace Yuni;

namespace GenericTools
{

	ReadParameterFile::ReadParameterFile(LoggingFacility& logs, const YString& file,
		const std::list<KeyString>& expectedKeys)
		: logs(logs),
		  pFile(file)
	{
		setKeys(expectedKeys);

		// Assign values from the parameter file
		String key, value, line;
		auto end = pParameters.end();

		IO::File::Stream inputStream;

		if (!inputStream.open(pFile))
		{
			logs.fatal() << "Unable to read file " << pFile;
			throw std::exception();
		}

		while (inputStream.readline(line))
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
		}


		if (!checkParameters())
			throw std::exception();
	}


	bool ReadParameterFile::checkParameters() const
	{
		for (auto it = pParameters.cbegin(), end = pParameters.cend(); it != end; ++it)
		{
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

		if (it == pParameters.end())
		{
			logs.fatal("Parameter \"") << key << "\" was not foreseen";
			exit(EXIT_FAILURE);
		}

		return it->second;
	}


	void ReadParameterFile::setKeys(const std::list<KeyString>& keys)
	{
		for (auto it = keys.cbegin(), end = keys.cend(); it != end; ++it)
			pParameters[*it] = "";
	}


} // namespace GenericTools
