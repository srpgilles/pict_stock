#include "photo_directory.hpp"
#include "extended_photo.hpp"

using namespace Yuni;

namespace PictStock
{


	PhotoDirectory::PhotoDirectory(LoggingFacility& logs, const Yuni::String& folder, const YString& strPathFormat)
		: logs(logs),
		  pMainFolder(folder),
		  pPathFormat(new Private::PathFormat(logs, strPathFormat))
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



	bool PhotoDirectory::createFolder(YString& folder, const ExtendedPhoto& photo)
	{
		assert(!(!pPathFormat));
		auto& pathFormat = *pPathFormat;
		pathFormat.determineMinimalFolder(folder, photo);

		if ((!IO::Directory::Exists(folder)) && (!IO::Directory::Create(folder)))
			return false;

		// Add new folder in the tree
		auto infosPtr = photo.informations();
		assert(!(!infosPtr));
		Private::RelevantInformations onlyUsefulInfos =
			infosPtr->onlyUsefulOnes(pathFormat.folderContent());
		pTree.insert(std::make_pair(onlyUsefulInfos, folder));

		return true;
	}
		


} // namespace SgPḧoto
