#include "photo_directory.hpp"
#include "private/photo_directory_iterator.hpp"
#include "../extended_photo/path_informations.hpp"

using namespace Yuni;

namespace PictStock
{


	PhotoDirectory::PhotoDirectory(LoggingFacility& logs, const Yuni::String& folder, const YString& strPathFormat)
		: logs(logs),
		  pMainFolder(folder),
		  pPathFormat(new PathFormat(logs, strPathFormat))
	{
		assert(!(!pPathFormat));
		auto& pathFormat = *pPathFormat;

		Private::PhotoDirectoryIterator iterator(logs, pathFormat);
		iterator.add(folder);

		if (!iterator.start())
		{
			logs.fatal() << "Problem encountered while scanning photo directory";
			exit(EXIT_FAILURE);
		}

		iterator.wait();
		iterator.validDirectories(pTree);
	}


	PhotoDirectory::~PhotoDirectory()
	{ }



	bool PhotoDirectory::createFolder(YString& folder, const PathInformations& infos)
	{
		assert(!(!pPathFormat));

		auto& pathFormat = *pPathFormat;

		pathFormat.determineMinimalFolder(folder, pMainFolder, infos);

		if ((!IO::Directory::Exists(folder)) && (!IO::Directory::Create(folder)))
			return false;

		// Add new folder in the tree
		PathInformations onlyUsefulInfos(logs);
		pathFormat.onlyUsefulFolderElements(onlyUsefulInfos, infos);

		pTree.insert(std::make_pair(onlyUsefulInfos, folder));

		return true;
	}
		


} // namespace SgPḧoto
