#include "sort_new_photos.hpp"
#include "private/populate_day_folder.hpp"
#include <yuni/io/directory/info.h>

using namespace Yuni;

namespace PictStock
{

	SortNewPhotos::SortNewPhotos(LoggingFacility& logs, const String& inputDirectory,
		PhotoDirectory& photoDirectory, const String& summaryFile,
		bool doFolderManuallyDate)
		: logs(logs),
		  pPhotoDirectory(photoDirectory),
		  pInputDirectory(inputDirectory),
		  pSummaryFile(summaryFile)
	{
		Private::SortNewPhotosIterator iterator(logs, inputDirectory, doFolderManuallyDate);
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

			{
				Private::PopulateDayFolder populateFolder(logs, targetFolder, folderDate,
					it->second, pSummaryFile);
				if (!populateFolder.proceed())
					return false;
			}
		}
		return true;

	}


} // namespace PictStock
