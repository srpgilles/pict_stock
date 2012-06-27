#include "pict_frame.hpp"


namespace PictStock
{

	PictFrame::PictFrame(LoggingFacility& logs, const PathFormat& pathFormat,
		const YString& photoDirectory, const YString& outputDirectory,
		unsigned int nbPhotos, const time_t beginDate, const time_t endDate, ReadDate::Mode mode)
	{
		Private::ScanPhotoDirectory helper(logs, pathFormat, photoDirectory,
			beginDate, endDate, mode);
		auto pool = helper.pool();

		logs.notice() << "There are " << pool.size() << " photos in the pool; "
			<< "among which " << nbPhotos << " will be selected";






	}



} // namespace PictStock
