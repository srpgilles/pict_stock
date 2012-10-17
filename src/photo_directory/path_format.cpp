
#include "path_format.hpp"
#include <yuni/io/file.h>
#include "../extended_photo/extended_photo.hpp"
#include "../tools/exceptions.hpp"

using namespace Yuni;

# ifdef USE_BOOST_REGULAR_EXPR
namespace regexNS = boost;
# else
namespace regexNS = std;
# endif // USE_BOOST_REGULAR_EXPR

namespace PictStock
{
namespace PhotoDirectory
{

	PathFormat::PathFormat(LoggingFacility& logs, const AnyString& format)
		: logs(logs),
		  pFolderPart(nullptr),
		  pFilePart(nullptr)
	{
		if (format.contains('('))
			throw GenericTools::Exception("Format shouldn't include any parenthesis");

		{
			String folderName, fileName;

			// Split the path and the filename
			IO::ExtractFilePath(folderName, format, false);

			if (folderName.empty())
				fileName = format;
			else
			{
				IO::ExtractFileName(fileName, format, false);
				if (IO::Separator != '/')
				{
					if (IO::Separator == '\\')
						folderName.replace("/", "\\\\");
					else
						folderName.replace('/', IO::Separator);
				}
				logs.notice("Folder = ") << folderName;
				pFolderPart = new Private::PathFormatHelper(logs, folderName);
			}

			logs.notice("File = ") << fileName;
			pFilePart = new Private::PathFormatHelper(logs, fileName);
		}
	}


	bool PathFormat::doFolderMatch(const AnyString& path, ExtendedPhoto::PathInformations& infos) const
	{
		// Trivial case: if no folder defined any path match
		if (!pFolderPart)
			return true;

		return pFolderPart->isOk(path, infos);
	}




	void PathFormat::determineMinimalFolder(Yuni::String& out,
		const YString& mainFolder, const ExtendedPhoto::ExtendedPhoto& photo) const
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
		const YString& mainFolder, const ExtendedPhoto::PathInformations& infos) const
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
		const ExtendedPhoto::ExtendedPhoto& photo) const
	{
		assert(!(!pFilePart));
		pFilePart->determineMinimalPath(out, photo);
	}


	void PathFormat::onlyUsefulFolderElements(ExtendedPhoto::PathInformations& out,
		const ExtendedPhoto::PathInformations& input) const
	{
		assert(!(!pFolderPart));
		pFolderPart->onlyUsefulElements(out, input);
	}


	void PathFormat::onlyUsefulFolderElements(ExtendedPhoto::PathInformations& out,
		const ExtendedPhoto::ExtendedPhoto& input) const
	{
		assert(!(!pFolderPart));
		pFolderPart->onlyUsefulElements(out, input);
	}


} // namespace PhotoDirectory
}// namespace PictStock
