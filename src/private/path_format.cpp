
#include "path_format.hpp"
#include <yuni/io/file.h>


using namespace Yuni;

namespace PictStock
{
namespace Private
{


	const Element::Vector PathFormat::pElements =
		{
			new Element(Elements::year, "%y", "\\d{4}"), // year
			new Element(Elements::month, "%m", " \\d{1}|0\\d{1}|10|11|12"), // month
			new Element(Elements::day, "%d", "[ |0|1|2]\\d{1}|3[0|1]"), // day
			new Element(Elements::hour, "%H", "[ |0|1]\\d{1}|2[0-4]|  "), // hour
			new Element(Elements::minute, "%M", "[ 0-5]\\d{1}|  "), // minute
			new Element(Elements::second, "%S", "[ 0-5]\\d{1}|  "), // second
			new Element(Elements::photographer, "%P", ".*") // photographer
		};




	PathFormatException::PathFormatException(const AnyString& msg)
		: pMessage(msg)
	{ }


	PathFormatException::~PathFormatException() throw()
	{ }


	const char* PathFormatException::what() const throw()
	{
		return pMessage.c_str();
	}


	PathFormat::PathFormat(LoggingFacility& logs, const AnyString& format)
		: logs(logs)
	{
		String path, filename;

		if (format.contains('('))
			throw PathFormatException("Format shouldn't include any parenthesis");

		{
			// Split the path and the filename
			IO::ExtractFilePath(path, format);

			if (path.empty())
				filename = format;
			else
				IO::ExtractFileName(filename, format);
		}

		if (path.notEmpty())
		{
			// Create the regular expression used to match the folders
			setRegExFolder(path);
		}

		{
			// Keep here expected form of the filename


		}

	}

	void PathFormat::setRegExFolder(const String& path)
	{
		assert(path.notEmpty());
		assert(pDoFolderContains.size() == pElements.size());

		{
			// First determine the ordering of the symbols found in user-defined expression
			typedef std::map<unsigned int, Element::Ptr> Position_Symbol;
			Position_Symbol positions;

			for (size_t i = 0u, size = pElements.size(); i < size; ++i)
			{
				auto elementPtr = pElements[i];
				assert(!(!elementPtr));
				auto& element = *elementPtr;
				auto pos = path.indexOf(0, element.symbol());

				if (pos != String::npos)
				{
					positions[pos] = elementPtr;
					pDoFolderContains[element.nature] = true;
				}
				// no else: by default a bitset is filled with false everywhere
			}

			assert(pSymbolOrdering.empty() && "Method should only be called once in construction");
			pSymbolOrdering.reserve(positions.size() + 1u); // +1u as first is deliberately left empty
			pSymbolOrdering.push_back(nullptr);

			for (auto it = positions.cbegin(), end = positions.cend(); it != end; ++it)
			{
				assert(!(!(it->second)));
				pSymbolOrdering.push_back(it->second);
			}
		}

		{
			// Determine regex expression by replacing symbols by adequate expression
			// If one is present more than once, only the first one is enclosed within parenthesis
			// (due to the way regex_results works)

			String helper(path);

			for (size_t i = 1u, size = pSymbolOrdering.size(); i < size; ++i)
			{
				// We must use a trick as some useful string methods don't exist
				// (count the number of occurrence of a given substring, or replace only
				// first occurrence)
				auto elementPtr = pSymbolOrdering[i];
				assert(!(!elementPtr));
				auto& element = *elementPtr;
				auto symbol = element.symbol();
				auto regex = element.regex();
				unsigned int pos = helper.find(symbol);
				assert(pos != String::npos && "If equal, something went wrong above");

				// First replace second and more occurrences
				helper.replace(pos + 1u, symbol, regex);

				// Now replace the first expression
				String buf('(');
				buf << regex << ')';
				helper.replace(symbol, buf);
			}

			pRegExFolder = boost::regex(helper.c_str());
		}
	}


	bool PathFormat::isOk(const AnyString& path, boost::cmatch& m) const
	{
		return (regex_search(path.c_str(), m, pRegExFolder));
	}


}// namespace Private
}// namespace PictStock
