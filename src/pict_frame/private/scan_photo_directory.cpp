#include "scan_photo_directory.hpp"
#include "../../photo_directory/path_format.hpp"
#include "../../extended_photo/path_informations.hpp"
#include "../../extended_photo/extended_photo.hpp"


using namespace Yuni;

namespace PictStock
{
namespace Private
{


	ScanPhotoDirectory::ScanPhotoDirectory(LoggingFacility& logs,
		const PathFormat& pathFormat,
		const time_t beginDate,
		const time_t endDate,
		ReadDate::Mode mode)
		: logs(logs),
		  pBeginDate(Date(beginDate)),
		  pEndDate(Date(endDate)),
		  pMode(mode),
		  pPathFormat(pathFormat)
	{ }


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
		if (checkValidity(completePath))
			return IO::flowContinue;
		
		return IO::flowSkip;
	}

	void ScanPhotoDirectory::onEndFolder(const String&, const String&, const String&)
	{ }


	IO::Flow ScanPhotoDirectory::onFile(const String&  completePath,
		const String& /*folder*/, const String& /*name*/, uint64 /*size*/)
	{
		if (pMode == ReadDate::safe)
		{
			// We load the photo to check the date. We could imagine later to
			// provide a fast option which prevent this step (which shouldn't
			// modify anything in case folder actually support most of the date
			// information and photo directory was generated with PictStock;
			// however if not the case this step is really required)
			ExtendedPhoto photo(logs, completePath);
			auto ptr = photo.informations();
			assert(!(!ptr));
			Date photoDate = ptr->date();

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
		PathInformations usefulInformations(logs);

		if (pPathFormat.doFolderMatch(directory, usefulInformations))
		{
			Date folderDate = usefulInformations.date();
			return ((folderDate >= pBeginDate) && (folderDate <= pEndDate));
		}

		return false;
	}


	inline const std::deque<YString>& ScanPhotoDirectory::pool() const
	{
		return pValidPhotos;
	}

} // namespace Private
} // namespace PictStock
