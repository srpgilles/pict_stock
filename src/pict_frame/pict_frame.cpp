#include "pict_frame.hpp"
#include "../tools/exceptions.hpp"
#include <yuni/io/directory/info/info.h>
#include <yuni/core/math/random/default.h>


using namespace Yuni;

namespace PictStock
{
	namespace
	{
		/*!
		** \brief Compare dates of two extended photos object
		*/
		bool sortByDate(const ExtendedPhoto::Ptr& photo1, const ExtendedPhoto::Ptr& photo2)
		{
			auto infos1Ptr = photo1->informations();
			auto infos2Ptr = photo2->informations();

			assert(!(!infos1Ptr));
			assert(!(!infos2Ptr));

			return (infos1Ptr->date() < infos2Ptr->date());
		}

	} // namespace anonymous



	PictFrame::PictFrame(LoggingFacility& logs, const PathFormat& pathFormat,
		const YString& photoDirectory, const YString& outputDirectory,
		unsigned int nbPhotos, const time_t beginDate, const time_t endDate,
		ReadDate::Mode mode, bool isChronological)
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
		selectPhotos(pool);

		// If requested shuffle all selected photos
		if (isChronological)
			pPhotosChosen.sort(sortByDate);
	}


	void PictFrame::selectPhotos(const std::deque<YString>& pool)
	{
		unsigned int poolSize = static_cast<unsigned int>(pool.size());
		logs.notice() << "There are " << poolSize << " photos in the pool; "
			<< "among which " << pNbPhotos << " will be selected";

		if (poolSize <= pNbPhotos)
		{
			// Trivial case: keep all photos!
			for (auto it = pool.cbegin(), end = pool.cend(); it != end; ++it)
			{
				ExtendedPhoto::Ptr ptr = new ExtendedPhoto(logs, *it);
				pPhotosChosen.push_back(ptr);
			}
		}
		else
		{
			/*
			 *  Algorithm to implement:
			 *
			 *  . Calculate the distance
			 *  . Generate a random number in [0, distance[ to determine the starting
			 *  point of the sequence
			 *  . If the sequence is longer than the requested number of elements,
			 *  eliminate randomly as many elements as necessary. To do so, perform
			 *  shuffle and then delete last entries for the sake of efficiency
			 *  . And if user want chronological order, it is not a big deal to sort
			 *  again the vector with a proper predicate
			 *
			 *
			 */

			// Calculate the "distance" between two photos to be kept
			// For instance, if we want 3 photos out of 10, distance is 10/3 = 3
			unsigned int dist = static_cast<unsigned int>(poolSize) / pNbPhotos;

			// Generate a random number in [0, distance[ to determine the starting
			// point of the sequence; generate from there the sequence of indexes to
			// consider
			std::vector<unsigned int> randomSequence;

			{
				randomSequence.reserve(1u + poolSize / dist); // upper bound of maximum size
				Math::Random::Default random;
				unsigned int startingIndex = static_cast<unsigned int>(random.next()) % dist;

				for (unsigned int i = startingIndex; i < poolSize; i += dist)
					randomSequence.push_back(i);
			}


			{
				// If too many elements, shuffle the list and remove as many last elements as
				// necessary
				unsigned int nbElementsToDelete;

				{
					unsigned int lengthSeq = static_cast<unsigned int>(randomSequence.size());
					assert(lengthSeq >= pNbPhotos && "If not, bug somewhere in PictFrame::selectPhotos");
					nbElementsToDelete = lengthSeq - pNbPhotos;
				}

				if (nbElementsToDelete > 0u)
				{
					std::random_shuffle(randomSequence.begin(), randomSequence.end());
					while (nbElementsToDelete-- > 0u)
						randomSequence.pop_back();
				}

				assert(randomSequence.size() == pNbPhotos);
			}

			{
				// Finally create the list from the index generated above
				for (auto it = randomSequence.cbegin(), end = randomSequence.cend();
					it != end; ++it)
				{
					assert(*it < poolSize);
					ExtendedPhoto::Ptr ptr = new ExtendedPhoto(logs, pool[*it]);
					pPhotosChosen.push_back(ptr);
				}
			}
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
