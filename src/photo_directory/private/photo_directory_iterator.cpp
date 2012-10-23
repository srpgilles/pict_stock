#include "photo_directory_iterator.hpp"
#include "../path_format.hpp"
#include "../../extended_photo/path_informations.hpp"
#include "../../database/cameras.hpp"

using namespace Yuni;

namespace PictStock
{
namespace PhotoDirectory
{
namespace Private
{


	PhotoDirectoryIterator::PhotoDirectoryIterator(LoggingFacility& logs,
		const ExtendedPhoto::Cameras& cameras, const PathFormat& pathFormat)
		: logs(logs),
		  pFolderCount(0u),
		  pValidFolderCount(0u),
		  pPathFormat(pathFormat),
		  pCameras(cameras)
	{ }


	PhotoDirectoryIterator::~PhotoDirectoryIterator()
	{
		// For code robustness and to avoid corrupt vtable
		stop();
	}


	void PhotoDirectoryIterator::validDirectories(ValidDirectoriesType& list)
	{
		list = pValidDirectories;
	}


	bool PhotoDirectoryIterator::onStart(const String& rootFolder)
	{
		logs.info() << " [+] " << rootFolder;

		return true;
	}


	PhotoDirectoryIterator::Flow PhotoDirectoryIterator::onBeginFolder(const String& completePath,
		const String&, const String& /*name*/)
	{
		if (checkValidity(completePath))
			++pValidFolderCount;

		++pFolderCount;
		
		return IO::flowContinue;
	}

	void PhotoDirectoryIterator::onEndFolder(const String&, const String&, const String&)
	{ }

	PhotoDirectoryIterator::Flow PhotoDirectoryIterator::onFile(const String&,
		const String& /*folder*/, const String& /*name*/, uint64 /*size*/)
	{
		return IO::flowContinue;
	}

	void PhotoDirectoryIterator::onTerminate()
	{
		logs.info() << pValidFolderCount << " / " << pFolderCount
			<< " folder(s) are valid";
	}


	bool PhotoDirectoryIterator::checkValidity(const String& directory)
	{
		ExtendedPhoto::PathInformations usefulInformations(logs, pCameras);

		if (pPathFormat.doFolderMatch(directory, usefulInformations))
		{
			pValidDirectories.insert(std::make_pair(usefulInformations, directory));
			return true;
		}

		return false;
	}

} // namespace Private
} // namespace PhotoDirectory
} // namespace PictStock
