
#include "path_format.hpp"
#include <yuni/io/file.h>
#include "../extended_photo.hpp"


using namespace Yuni;

namespace PictStock
{
namespace Private
{


	const Traits::Element::Vector PathFormatHelper::pElements =
		{
			new Traits::Year(),
			new Traits::Month(),
			new Traits::Day(),
			new Traits::Hour(),
			new Traits::Minute(),
			new Traits::Second(),
			new Traits::Photographer()
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


	PathFormatHelper::PathFormatHelper(LoggingFacility& logs, const AnyString& format)
		: logs(logs),
		  pFormat(format)
	{
		setRegEx();
	}


	void PathFormatHelper::setRegEx()
	{
		const auto& path = pFormat;
		assert(path.notEmpty());
		assert(doContains.size() == pElements.size());

		{
			// First determine the ordering of the symbols found in user-defined expression
			typedef std::map<unsigned int, Traits::Element::Ptr> Position_Symbol;
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
					doContains[element.nature] = true;
				}
				// no else: by default a bitset is filled with false everywhere
			}

			assert(pSymbolOrdering.empty() && "Method should only be called once in construction");
			pSymbolOrdering.reserve(positions.size());

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

				helper.replace(pos + 1u, symbol, regex);

				// Now replace the first expression
				String buf('(');
				buf << regex << ')';
				helper.replace(symbol, buf);
			}

			pRegEx = boost::regex(helper.c_str());
		}
	}


	bool PathFormatHelper::isOk(const AnyString& path,
		std::map<Traits::Element::Ptr, CString<10, false> >& values) const
	{
		assert(values.size() == 0u);
		boost::cmatch m;

		if (!regex_search(path.c_str(), m, pRegEx))
			return false;

		size_t size = pSymbolOrdering.size();

		for (unsigned int i = 0u; i < size; ++i)
		{
			values[pSymbolOrdering[i]] = m[i + 1u].str();
			// + 1u because regex elements begin at 1; 0 is the full expression
		}

		return true;
	}


	void PathFormatHelper::determineMinimalPath(String& out,
		const RelevantInformations& infos) const
	{
		assert(out.empty());

		out = pFormat;

		for (auto it = pElements.cbegin(), end = pElements.cend(); it != end; ++it)
		{
			Traits::Element::Ptr elementPtr = *it;
			assert(!(!elementPtr));
			const Traits::Element& element = *elementPtr;

			if (doContains.test(element.nature))
				out.replace(element.symbol(), infos.value(element.nature));
		}
	}


	void PathFormatHelper::determineMinimalPath(Yuni::String& out,
		const ExtendedPhoto& photo) const
	{
		RelevantInformations::Ptr infosPtr = photo.informations();
		assert(!(!infosPtr));
		auto& infos = *infosPtr;
		determineMinimalPath(out, infos);
	}


	PathFormat::PathFormat(LoggingFacility& logs, const AnyString& format)
		: logs(logs),
		  pFolderPart(nullptr),
		  pFilePart(nullptr)
	{
		if (format.contains('('))
			throw PathFormatException("Format shouldn't include any parenthesis");

		{
			String folderName, fileName;

			// Split the path and the filename
			IO::ExtractFilePath(folderName, format);

			if (folderName.empty())
				fileName = format;
			else
			{
				IO::ExtractFileName(fileName, format);
				if (IO::Separator != '/')
					folderName.replace('/', IO::Separator);

				pFolderPart = new PathFormatHelper(logs, folderName);
			}

			pFilePart = new PathFormatHelper(logs, fileName);
		}
	}


	bool PathFormat::doFolderMatch(const AnyString& path,
		std::map<Traits::Element::Ptr, CString<10, false> >& out) const
	{
		// Trivial case: if no folder defined any path match
		if (!pFolderPart)
			return true;

		return pFolderPart->isOk(path, out);
	}


	void PathFormat::determineMinimalFolder(Yuni::String& out, const ExtendedPhoto& photo) const
	{
		assert(out.empty());

		if (!pFolderPart)
			return ;

		pFolderPart->determineMinimalPath(out, photo);
	}

	void PathFormat::determineMinimalFolder(Yuni::String& out, const RelevantInformations& infos) const
	{
		assert(out.empty());

		if (!pFolderPart)
			return ;

		pFolderPart->determineMinimalPath(out, infos);
	}


	void PathFormat::determineMinimalFilename(Yuni::String& out, const ExtendedPhoto& photo) const
	{
		assert(!(!pFilePart));

		pFilePart->determineMinimalPath(out, photo);
	}


	const std::bitset<Elements::size>& PathFormat::folderContent() const
	{
		assert(!(!pFolderPart));
		return pFolderPart->doContains;
	}



}// namespace Private
}// namespace PictStock
