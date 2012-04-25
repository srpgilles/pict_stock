#include "sort_new_photos.hpp"
#include "private/populate_day_folder.hpp"
#include <yuni/io/directory/info.h>

using namespace Yuni;

namespace SgPhoto
{


	SortNewPhotos::SortNewPhotos(LoggingFacility& logs, const String& inputDirectory,
		PhotoDirectory& photoDirectory, bool doFolderManuallyDate)
		: logs(logs),
		  pPhotoDirectory(photoDirectory),
		  pInputDirectory(inputDirectory)
	{
		Private::SortNewPhotosIterator iterator(logs, inputDirectory, doFolderManuallyDate);
		iterator.picturesToProcess(pPicturesToProcess);
	}


	SortNewPhotos::~SortNewPhotos()
	{  }


	bool SortNewPhotos::proceed()
	{
		// Go through #pPicturesToProcess, that includes all new pictures sort by date
		// (either exif date or manual date depending on input constructor parameter
		// doFolderManuallyDate)
		logs.warning() << "COPY INSTEAD OF MOVE DURING DEV";

		for (auto it = pPicturesToProcess.begin(), end = pPicturesToProcess.end();
			it != end; ++it)
		{
			const DateString& folderDate = it->first;

			std::list<YString> folders;
			YString targetFolder;

			if (!pPhotoDirectory.findDate(folderDate, folders))
			{
				if (!pPhotoDirectory.createDateFolder(folderDate, targetFolder))
				{
				  logs.error() << "Unable to create folder related to date " << folderDate;
				  return false;
				}
			}
			else
			{
				// TODO LATER
				// One or more output folders already exist; see which one to use
				// (or possibly create a new one)

				// FOR NOW, use default one
				// We don't care about return value: if not existing it is created
				// and added to the tree, if existing false is returned but folder
				// correctly set
				pPhotoDirectory.createDateFolder(folderDate, targetFolder);
			}

			Private::PopulateDayFolder(logs, targetFolder, folderDate, it->second);
		}
		return true;

	}


} // namespace SgPhoto
