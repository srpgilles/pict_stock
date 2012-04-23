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





} // namespace SgPhoto
