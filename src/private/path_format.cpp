
#include "path_format.hpp"
#include <yuni/io/file.h>


using namespace Yuni;

namespace PictStock
{
namespace Private
{
	const Element::Vector PathFormat::pElements =
		{
			new Element("%y", "\\d{4}"), // year
			new Element("%m", " \\d{1}|0\\d{1}|10|11|12"), // month
			new Element("%d", "[ |0|1|2]\\d{1}|3[0|1]"), // day
			new Element("%H", "[ |0|1]\\d{1}|2[0-4]|  "), // hour
			new Element("%M", "[ 0-5]\\d{1}|  "), // minute
			new Element("%S", "[ 0-5]\\d{1}|  "), // second
			new Element("%P", ".*") // photographer
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

	template<class TraitsT>
	void PathFormat::setRegExFolderHelper(String& path)
	{
		//auto pos = path.indexOf(0, TraitsT::Symbol());

//		if (pos != String::npos)
//			buf[pos] = i;

	}

	void PathFormat::setRegExFolder(const String& path)
	{
		assert(path.notEmpty());
		String helper(path);


		{
			// First determine the ordering of the symbols found in user-defined expression
			typedef std::map<unsigned int, Element::Ptr> Position_Symbol;
			Position_Symbol positions;

			for (size_t i = 0u, size = pElements.size(); i < size; ++i)
			{
				auto element = pElements[i];
				assert(!(!element));
				auto pos = path.indexOf(0, element->symbol());

				if (pos != String::npos)
				{
					logs.notice() << pos;
					positions[pos] = element;
				}
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

		logs.notice("HERE");
		for (size_t i = 1u, size = pSymbolOrdering.size(); i < size; ++i)
		{
			auto foo = pSymbolOrdering[i];
			assert(!(!foo));
			logs.warning() << foo->symbol();
		}
		logs.notice("THERE");


		pRegExFolder = boost::regex(helper.c_str());
	}


	bool PathFormat::isOk(const AnyString& path, boost::cmatch& m) const
	{
		return (regex_search(path.c_str(), m, pRegExFolder));
	}


}// namespace Private
}// namespace PictStock
