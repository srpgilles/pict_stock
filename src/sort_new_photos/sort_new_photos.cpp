#include "sort_new_photos.hpp"
#include "private/populate_day_folder.hpp"
#include <yuni/io/directory/info.h>

using namespace Yuni;

namespace PictStock
{
namespace SortNewPhotos
{

	SortNewPhotos::SortNewPhotos(LoggingFacility& logs,
		const ExtendedPhoto::Cameras& cameras,
		const String& inputDirectory,
		PhotoDirectory::PhotoDirectory& photoDirectory,
		const String& summaryFile,
		bool doFolderManuallyDate)
		: logs(logs),
		  pPhotoDirectory(photoDirectory),
		  pInputDirectory(inputDirectory),
		  pSummaryFile(summaryFile),
		  pCameras(cameras)
	{
		auto pathFormatPtr = pPhotoDirectory.pathFormat();
		assert(!(!pathFormatPtr));

		Private::SortNewPhotosIterator iterator(logs, cameras, inputDirectory, *pathFormatPtr, doFolderManuallyDate);
		iterator.picturesToProcess(pPicturesToProcess);
	}


	SortNewPhotos::~SortNewPhotos()
	{  }


	bool SortNewPhotos::proceed()
	{
		// Create summary file
		if (!IO::File::SetContent(pSummaryFile, "origin;target\n"))
		{
			logs.error() << "Unable to set content to file \"" << pSummaryFile << '"';
			return false;
		}


		// Go through #pPicturesToProcess, that includes all new pictures sort by date
		// (either exif date or manual date depending on input constructor parameter
		// doFolderManuallyDate)
		for (auto it = pPicturesToProcess.begin(), end = pPicturesToProcess.end();
			it != end; ++it)
		{
			const auto& folderInfos = it->first;

			std::list<YString> folders;
			YString targetFolder;

			if (!pPhotoDirectory.createFolder(targetFolder, folderInfos))
			{
				logs.error() << "Unable to create folder " << targetFolder;
				return false;
			}

			{
				auto pathFormatPtr = pPhotoDirectory.pathFormat();
				assert(!(!pathFormatPtr));

				Private::PopulateDayFolder populateFolder(logs, pCameras, *pathFormatPtr,
					targetFolder, folderInfos, it->second, pSummaryFile);
				if (!populateFolder.proceed())
					return false;
			}
		}
		return true;

	}

} // namespace SortNewPhotos
} // namespace PictStock
