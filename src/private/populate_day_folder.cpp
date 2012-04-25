#include <yuni/io/directory/info.h>

#include "populate_day_folder.hpp"

using namespace Yuni;

namespace SgPhoto
{
namespace Private
{

	PopulateDayFolder::PopulateDayFolder(LoggingFacility& logs, const YString& targetFolder,
		const DateString& targetDate, ExtendedPhoto::Vector& newPhotos)
		: logs(logs),
		  pTargetFolder(targetFolder),
		  pTargetDate(targetDate),
		  pNewPhotos(newPhotos)
	{
		// First of all, scan new photos and check their date matches targetDate
		// This should be the case in most cases; but in case you are sorting pictures
		// with unreliable exif data it might not be
		// If disagreement, modify the exif by giving a fake date associated with a proper comment
		enforceDateInNewPhotos();


//
//		{
//			// Load the photos possibly present in target folder in the map sorting
//			// the photo by date
//			IO::Directory::Info infoFolder(targetFolder);
//
//			auto it = infoFolder.file_begin();
//			auto end = infoFolder.file_end();
//
//			for (; it != end; ++it)
//			{
//				YString file = *it;
//
//				String ext;
//				IO::ExtractExtension(ext, file);
//
//				if (ext.toLower() != ".jpg")
//					continue;
//
//				ExtendedPhoto::Ptr photoPtr = new ExtendedPhoto(logs, file);
//				dayPictures[photoPtr->time()].push_back(photoPtr);
//			}
//		}

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

	}

	bool PopulateDayFolder::enforceDateInNewPhotos()
	{
		for (auto it = pNewPhotos.begin(), end = pNewPhotos.end(); it != end; ++it)
		{
			ExtendedPhoto::Ptr photoPtr = *it;
			assert(!(!photoPtr));
			ExtendedPhoto& photo = *photoPtr;

			if (photo.date() == pTargetDate)
				continue;

			logs.notice() << "DISCREPANCY for " << photo.originalPath();

		}

		return true;
	}


} // namespace Private
} // namespace SgPhoto
