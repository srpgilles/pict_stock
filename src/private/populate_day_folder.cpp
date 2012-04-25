#include "populate_day_folder.hpp"


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


	}


} // namespace Private
} // namespace SgPhoto
