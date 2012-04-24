#include <fstream>
#include "sort_new_photos.hpp"

using namespace Yuni;

namespace SgPhoto
{


	SortNewPhotos::SortNewPhotos(LoggingFacility& logs, const String& inputDirectory,
		PhotoDirectory& photoDirectory)
		: logs(logs),
		  pPhotoDirectory(photoDirectory),
		  pInputDirectory(inputDirectory)
	{
		Private::SortNewPhotosIterator iterator(logs, inputDirectory);
		iterator.picturesToProcess(pPicturesToProcess);
	}


	SortNewPhotos::~SortNewPhotos()
	{  }


	bool SortNewPhotos::proceed()
	{
		// TODO Adapt this code with the new conundrum (use key of pPicturesToProcess)
		//      // Check whether the date of the photo already exists or not
		//      auto photoDate = myPhoto.date();
		//      std::list<YString> folders;
		//      YString folder;
		//      if (!pPhotoDirectory.findDate(photoDate, folders))
		//      {
		//        // If the correct folder doesn't exist, just create it...
		//        if (!pPhotoDirectory.createDateFolder(photoDate, folder))
		//        {
		//          logs.error() << "Unable to create folder related to "
		//            "photo " << fullName;
		//          return IO::flowContinue;
		//        }
		//
		//        // ... and move the photo there
		//        logs.warning() << "COPY INSTEAD OF MOVE DURING DEV";
		//
		//
		//        YString targetFullPath(folder);
		//
		//        {
		//          YString targetName;
		//          myPhoto.newNameWithoutExtension(targetName);
		//          targetFullPath << IO::Separator << targetName << ".jpg";
		//        }
		//
		//        if (IO::File::Copy(fullName, targetFullPath, false) != IO::errNone)
		//        {
		//          logs.error() << "Unable to copy " << fullName
		//            << " into new folder " << folder;


	}





} // namespace SgPhoto
