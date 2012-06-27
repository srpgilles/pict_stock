#include "pict_frame.hpp"
#include "../tools/exceptions.hpp"
#include <yuni/io/directory/info/info.h>

using namespace Yuni;

namespace PictStock
{

	PictFrame::PictFrame(LoggingFacility& logs, const PathFormat& pathFormat,
		const YString& photoDirectory, const YString& outputDirectory,
		unsigned int nbPhotos, const time_t beginDate, const time_t endDate,
		ReadDate::Mode mode, bool doShuffle)
		: logs(logs),
		  pNbPhotos(nbPhotos)
	{
		prepareOutputDirectory(outputDirectory);

		Private::ScanPhotoDirectory helper(logs, pathFormat, photoDirectory,
			beginDate, endDate, mode);
		const auto& pool = helper.pool();

		selectPhotos(outputDirectory, pool, doShuffle);


	}

	void PictFrame::selectPhotos(const YString& outputDirectory,
		const std::deque<YString>& pool, bool doShuffle)
	{
		auto size = pool.size();
		logs.notice() << "There are " << pool.size() << " photos in the pool; "
			<< "among which " << pNbPhotos << " will be selected";

		if (size <= pNbPhotos)
		{
			// Trivial case: keep all photos!
			pPhotosChosen.reserve(size);


		}




	}


	void PictFrame::prepareOutputDirectory(const YString& outputDirectory) const
	{
		// If it doesn't exist, create it
		if (!IO::Directory::Exists(outputDirectory))
		{
			if (!(IO::Directory::Create(outputDirectory)))
				throw GenericTools::Exception("Unable to create output directory");
		}
		else
		{
			// Delete any pictures that may already exist in this folder

			IO::Directory::Info infoFolder(outputDirectory);

			auto it = infoFolder.file_begin();
			auto end = infoFolder.file_end();

			for (; it != end; ++it)
			{
				YString file(outputDirectory);
				file << IO::Separator << *it;

				String ext;
				if (!IO::ExtractExtension(ext, file))
					continue;

				if (ext.toLower() != ".jpg")
					continue;

				if (IO::File::Delete(file) != IO::errNone)
				{
					YString msg("Unable to delete file \"");
					msg << file << '"';
					throw GenericTools::Exception(msg);
				}
			}
		}
	}



} // namespace PictStock
