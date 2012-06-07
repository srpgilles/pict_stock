
#include "path_format.hpp"
#include <yuni/io/file.h>
#include "../extended_photo/extended_photo.hpp"


using namespace Yuni;

# ifdef USE_BOOST_REGULAR_EXPR
namespace regexNS = boost;
# else
namespace regexNS = std;
# endif // USE_BOOST_REGULAR_EXPR

namespace PictStock
{


	PathFormat::PathFormat(LoggingFacility& logs, const AnyString& format)
		: logs(logs),
		  pFolderPart(nullptr),
		  pFilePart(nullptr)
	{
		if (format.contains('('))
			throw Private::PathFormatException("Format shouldn't include any parenthesis");

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

				pFolderPart = new Private::PathFormatHelper(logs, folderName);
			}

			pFilePart = new Private::PathFormatHelper(logs, fileName);
		}
	}


	bool PathFormat::doFolderMatch(const AnyString& path, PathInformations& infos) const
	{
		// Trivial case: if no folder defined any path match
		if (!pFolderPart)
			return true;

		return pFolderPart->isOk(path, infos);
	}




	void PathFormat::determineMinimalFolder(Yuni::String& out,
		const YString& mainFolder, const ExtendedPhoto& photo) const
	{
		assert(out.empty());

		if (!pFolderPart)
			return ;

		YString buf;
		pFolderPart->determineMinimalPath(buf, photo);

		out = mainFolder;
		out << IO::Separator << buf;

	}


	void PathFormat::determineMinimalFolder(Yuni::String& out,
		const YString& mainFolder, const PathInformations& infos) const
	{
		assert(out.empty());

		if (!pFolderPart)
			return ;

		YString buf;
		pFolderPart->determineMinimalPath(buf, infos);

		out = mainFolder;
		out << IO::Separator << buf;
	}


	void PathFormat::determineMinimalFilename(Yuni::String& out,
		const ExtendedPhoto& photo) const
	{
		assert(!(!pFilePart));

		pFilePart->determineMinimalPath(out, photo);
	}


	const std::bitset<Elements::size>& PathFormat::folderContent() const
	{
		assert(!(!pFolderPart));
		return pFolderPart->doContains;
	}



}// namespace PictStock
