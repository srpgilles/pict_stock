#include "scan_photo_directory.hpp"
#include "../../photo_directory/path_format.hpp"
#include "../../extended_photo/path_informations.hpp"
#include "../../extended_photo/extended_photo.hpp"
#include "../../database/cameras.hpp"

using namespace Yuni;

namespace PictStock
{
namespace PictFrame
{
namespace Private
{


	ScanPhotoDirectory::ScanPhotoDirectory(LoggingFacility& logs,
		const Database::Cameras& cameras,
		const PhotoDirectory::PathFormat& pathFormat,
		const YString& photoDirectory,
		const time_t beginDate,
		const time_t endDate,
		ReadDate::Mode mode)
		: logs(logs),
		  pBeginDate(ExtendedPhoto::Date(beginDate)),
		  pEndDate(ExtendedPhoto::Date(endDate)),
		  pMode(mode),
		  pPathFormat(pathFormat),
		  pIsValidFolder(false),
		  pCameras(cameras)
	{
		if (!IO::Directory::Exists(photoDirectory))
		{
			logs.fatal() << "Error: input directory " << photoDirectory
				<< " doesn't exists\n";
			exit(-1); // TODO proper error handling to foresee
		}

		{
			// Now we want to iterate through the new files to add in the photo directory
			add(photoDirectory);
			start();
			wait();
		}
	}


	ScanPhotoDirectory::~ScanPhotoDirectory()
	{
		// For code robustness and to avoid corrupt vtable
		stop();
	}


	bool ScanPhotoDirectory::onStart(const String& rootFolder)
	{
		logs.info() << " [+] " << rootFolder;

		return true;
	}


	IO::Flow ScanPhotoDirectory::onBeginFolder(
		const String& completePath,
		const String&, const String& /*name*/)
	{
		// I can't directly rule out the entire folder by a flowSkip, as this
		// invalid folder may include perfectly valid subfolders...
		pIsValidFolder = checkValidity(completePath);

		return IO::flowContinue;
	}

	void ScanPhotoDirectory::onEndFolder(const String&, const String&, const String&)
	{ }


	IO::Flow ScanPhotoDirectory::onFile(const String&  completePath,
		const String& /*folder*/, const String& /*name*/, uint64 /*size*/)
	{
		if (!pIsValidFolder)
			return IO::flowContinue;

		if (!ExtendedPhoto::isExtensionManaged(completePath))
			return IO::flowContinue;

		if (pMode == ReadDate::safe)
		{
			// We load the photo to check the date. We could imagine later to
			// provide a fast option which prevent this step (which shouldn't
			// modify anything in case folder actually support most of the date
			// information and photo directory was generated with PictStock;
			// however if not the case this step is really required)
			ExtendedPhoto::ExtendedPhoto photo(logs, pCameras, completePath);
			auto ptr = photo.informations();
			assert(!(!ptr));
			ExtendedPhoto::Date photoDate = ptr->date();

			if (photoDate < pBeginDate || photoDate > pEndDate)
				return IO::flowContinue;
		}

		pValidPhotos.push_back(completePath);

		return IO::flowContinue;
	}


	void ScanPhotoDirectory::onTerminate()
	{ }


	bool ScanPhotoDirectory::checkValidity(const String& directory)
	{
		ExtendedPhoto::PathInformations usefulInformations(logs, pCameras);

		if (pPathFormat.doFolderMatch(directory, usefulInformations))
		{
			ExtendedPhoto::Date folderDate = usefulInformations.date();
			return ((folderDate >= pBeginDate) && (folderDate <= pEndDate));
		}

		return false;
	}


	const std::deque<YString>& ScanPhotoDirectory::pool() const
	{
		return pValidPhotos;
	}

} // namespace Private
} // namespace PictFrame
} // namespace PictStock
