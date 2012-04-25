#include "sort_new_photos.hpp"
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

			std::map<HourString, ExtendedPhoto::Vector> dayPictures;

			{
				// Load the photos possibly present in target folder in the map sorting
				// the photo by date
				IO::Directory::Info infoFolder(targetFolder);

				auto it = infoFolder.file_begin();
				auto end = infoFolder.file_end();

				for (; it != end; ++it)
				{
					YString file = *it;

					String ext;
					IO::ExtractExtension(ext, file);

					if (ext.toLower() != ".jpg")
						continue;

					ExtendedPhoto::Ptr photoPtr = new ExtendedPhoto(logs, file);
					dayPictures[photoPtr->time()].push_back(photoPtr);
				}
			}

			// TODO WHAT IS LACKING: load pre-existing photo to avoid duplication and handle
			// multiple photos in the same minute

//			{
//				// Now move all associated photos to the target folder and rename them
//				ExtendedPhoto::Vector& photos = it->second;
//
//				for (auto it = photos.begin(), end = photos.end(); it != end; ++it)
//				{
//					auto photoPtr = *it;
//					assert(!(!photoPtr));
//					auto& photo = *photoPtr;
//
//					if (photo.date() != folderDate)
//					{
//						// Not the most usual case: exif does not match the folderDate,
//						// which can only happen in case of manual date entry.
//						// Modify the exif accordingly!
//						// TODO Add the modification!!!
//
//					}
//
//					if (dayPictures.find(photo.time()) == dayPictures.end())
//					{
//						// No picture with the same time, no risk of failing the copy due to
//						// previous existence of target
//
//
//						// Update dayPictures
//						dayPictures[photo.time()].push_back(photoPtr);
//					}
//
//
//
//					YString targetFullPathWithoutExtension(targetFolder);
//
//					{
//						YString targetName;
//						photo.newNameWithoutExtension(targetName);
//						targetFullPathWithoutExtension << IO::Separator << targetName;
//					}
//
//
//
//					if (IO::File::Copy(photo.originalPath(), targetFullPath, false) != IO::errNone)
//					{
//
//						logs.error() << "Unable to copy " << photo.originalPath()
//							<< " into new folder " << targetFolder;
//						return false;
//					}
//					else
//					{
//						logs.notice() << "OK to copy " << photo.originalPath()
//							<< " into new folder " << targetFolder;
//
//					}
//
//				}
//			}
		} // for loop over #pPicturesToProcess

		return true;

	}





} // namespace SgPhoto
