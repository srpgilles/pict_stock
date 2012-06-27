#include "pict_frame.hpp"
#include "../tools/exceptions.hpp"
#include <yuni/io/directory/info/info.h>
#include <yuni/core/math/random/range.h>
#include <yuni/core/math/random/default.h>


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
		// Create or empty (only jpg) target directory
		prepareOutputDirectory(outputDirectory);

		// Scan the photo directory from which the selection will be performed
		Private::ScanPhotoDirectory helper(logs, pathFormat, photoDirectory,
			beginDate, endDate, mode);
		const auto& pool = helper.pool();

		// Select randomly some photos from photo directory
		selectPhotos(outputDirectory, pool);

		// If requested shuffle all selected photos
		if (doShuffle)
			std::random_shuffle(pPhotosChosen.begin(), pPhotosChosen.end());

		// Copy the selected photos to the target folder and rename them


	}

	void PictFrame::selectPhotos(const YString& outputDirectory,
		const std::deque<YString>& pool)
	{
		auto size = pool.size();
		logs.notice() << "There are " << pool.size() << " photos in the pool; "
			<< "among which " << pNbPhotos << " will be selected";

		if (size <= pNbPhotos)
		{
			// Trivial case: keep all photos!
			pPhotosChosen.resize(size);
			std::copy(pool.cbegin(), pool.cend(), pPhotosChosen.begin());
		}
		else
		{
			pPhotosChosen.reserve(pNbPhotos);

			// Calculate the "distance" between two photos to be kept
			// For instance, if we want 3 photos out of 10, distance is 10/3 = 3.333
			double dist = static_cast<double>(size) / static_cast<double>(pNbPhotos);

			// Choose randomly the distance
			Math::Random::Range<Math::Random::Default> range;
			range.reset();

			range();



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
