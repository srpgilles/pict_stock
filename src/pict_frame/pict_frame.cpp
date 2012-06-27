#include "pict_frame.hpp"
#include "../tools/exceptions.hpp"

using namespace Yuni;

namespace PictStock
{

	PictFrame::PictFrame(LoggingFacility& logs, const PathFormat& pathFormat,
		const YString& photoDirectory, const YString& outputDirectory,
		unsigned int nbPhotos, const time_t beginDate, const time_t endDate, ReadDate::Mode mode)
	{
		// Create the output directory if it doesn't exist
		if (!IO::Directory::Exists(photoDirectory))
		{
			logs.fatal() << "Error: input directory " << photoDirectory
				<< " doesn't exists\n";
			exit(-1); // TODO proper error handling to foresee
		}


		Private::ScanPhotoDirectory helper(logs, pathFormat, photoDirectory,
			beginDate, endDate, mode);
		const auto& pool = helper.pool();

		logs.notice() << "There are " << pool.size() << " photos in the pool; "
			<< "among which " << nbPhotos << " will be selected";






	}


	void PictFrame::prepareOutputDirectory(const YString& outputDirectory) const
	{
		// If it doesn't exist, create it
		if (!IO::Directory::Exists(outputDirectory))
		{
			if (!(IO::Directory::Create(outputDirectory)))
				throw GenericTools::Exception("Unable to create output directory");
		}
	}



} // namespace PictStock
