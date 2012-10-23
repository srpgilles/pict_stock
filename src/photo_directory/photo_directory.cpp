#include "photo_directory.hpp"
#include "private/photo_directory_iterator.hpp"
#include "../extended_photo/path_informations.hpp"

using namespace Yuni;

namespace PictStock
{
namespace PhotoDirectory
{

    PhotoDirectory::PhotoDirectory(LoggingFacility& logs, const Database::Cameras& cameras,
		const Yuni::String& folder, const YString& strPathFormat)
		: logs(logs),
		  pMainFolder(folder),
		  pPathFormat(new PathFormat(logs, strPathFormat)),
		  pCameras(cameras)
	{
		assert(!(!pPathFormat));
		auto& pathFormat = *pPathFormat;

		Private::PhotoDirectoryIterator iterator(logs, pCameras, pathFormat);
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



	bool PhotoDirectory::createFolder(YString& folder, const ExtendedPhoto::PathInformations& infos)
	{
		assert(!(!pPathFormat));

		auto& pathFormat = *pPathFormat;

		pathFormat.determineMinimalFolder(folder, pMainFolder, infos);

		if ((!IO::Directory::Exists(folder)) && (!IO::Directory::Create(folder)))
			return false;

		// Add new folder in the tree
		ExtendedPhoto::PathInformations onlyUsefulInfos(logs, pCameras);
		pathFormat.onlyUsefulFolderElements(onlyUsefulInfos, infos);

		pTree.insert(std::make_pair(onlyUsefulInfos, folder));

		return true;
	}
		

} // namespace PhotoDirectory
} // namespace PictStock
