
#include "path_format.hpp"
#include <yuni/io/file.h>
#include "traits/traits.hpp"


using namespace Yuni;

namespace PictStock
{
namespace Private
{

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
		if (path.contains(TraitsT::Symbol()))
		{
			path.replace(TraitsT::Symbol(), TraitsT::Regex());
			pDoFolderContains[TraitsT::index] = true;
		}
	}

	void PathFormat::setRegExFolder(const String& path)
	{
		assert(path.notEmpty());
		String helper(path);
		setRegExFolderHelper<Traits::Year>(helper);
		setRegExFolderHelper<Traits::Month>(helper);
		setRegExFolderHelper<Traits::Day>(helper);
		setRegExFolderHelper<Traits::Hour>(helper);
		setRegExFolderHelper<Traits::Minute>(helper);
		setRegExFolderHelper<Traits::Second>(helper);
		setRegExFolderHelper<Traits::Photographer>(helper);

		pRegExFolder = boost::regex(helper.c_str());
	}


	bool PathFormat::isOk(const AnyString& path, boost::cmatch& m) const
	{
		return (regex_search(path.c_str(), m, pRegExFolder));
	}


}// namespace Private
}// namespace PictStock
