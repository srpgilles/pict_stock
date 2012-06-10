#include <yuni/io/directory/info.h>
#include <yuni/io/directory/system.h>
#include <yuni/datetime/timestamp.h>
#include <yuni/core/math.h>
#include <fstream>
#include "populate_day_folder.hpp"
#include "../../photo_directory/path_format.hpp"

using namespace Yuni;

namespace PictStock
{
namespace Private
{
	namespace // anonymous
	{
		// A function to determine the number of digits of an unsigned int
		unsigned int numberOfDigits(unsigned int number)
		{
			return static_cast<unsigned int>(Math::Ceil(log10(static_cast<double>(number + 1u))));
		}

	}// namespace anonymous


	PopulateDayFolder::PopulateDayFolder(LoggingFacility& logs,
		const PathFormat& pathFormat,
		const YString& targetFolder,
		const PathInformations& targetInfos,
		ExtendedPhoto::Vector& newPhotos,
		const YString& summaryFile)
		: logs(logs),
		  pTargetFolder(targetFolder),
		  pTargetInformations(targetInfos),
		  pPathFormat(pathFormat),
		  pNewPhotos(newPhotos),
		  pSummaryFile(summaryFile)
	{ }

	PopulateDayFolder::~PopulateDayFolder()
	{ }


	bool PopulateDayFolder::proceed()
	{
		// Load into #pPhotosPerName the photo already in target directory
		if (!insertExistingPhotos())
			return false;

		// Load new photos into #pPhotosPerName
		insertNewPhotos();

		// Arrange all those photos in targetFolder
		if (!putPhotosInTarget())
			return false;

		return true;
	}



	bool PopulateDayFolder::insertExistingPhotos()
	{
		IO::Directory::Info infoFolder(pTargetFolder); // lightweight Yuni way to go through a folder

		auto it = infoFolder.file_begin();
		auto end = infoFolder.file_end();

		for (; it != end; ++it)
		{
			YString file(pTargetFolder);
			file << IO::Separator << *it;

			String ext;
			if (!IO::ExtractExtension(ext, file))
				continue;

			if (ext.toLower() != ".jpg")
				continue;

			ExtendedPhoto::Ptr photoPtr = new ExtendedPhoto(logs, file);

			YString newName;
			pPathFormat.determineMinimalFilename(newName, *photoPtr);
			pPhotosPerName[newName].push_back(photoPtr);
		}

		return true;
	}


	void PopulateDayFolder::insertNewPhotos()
	{
		for (auto it = pNewPhotos.cbegin(), end = pNewPhotos.cend(); it != end; ++it)
		{
			ExtendedPhoto::Ptr photoPtr = *it;
			assert(!(!photoPtr));
			YString newName;
			ExtendedPhoto& photo = *photoPtr;
			pPathFormat.determineMinimalFilename(newName, *photoPtr);


			{
				ExtendedPhoto::Vector& allPhotos = pPhotosPerName[newName];

				unsigned int size = static_cast<unsigned int>(allPhotos.size());

				bool doPhotoAlreadyExist = false;
				for (unsigned int i = 0u; !doPhotoAlreadyExist && i < size; ++i)
				{
					auto ptr = allPhotos[i];
					assert(!(!ptr));

					if (*ptr == photo)
						doPhotoAlreadyExist = true;
				}

				if (!doPhotoAlreadyExist)
					pPhotosPerName[newName].push_back(photoPtr);
			}
		}
	}


	bool PopulateDayFolder::putPhotosInTarget()
	{
		for (auto it = pPhotosPerName.cbegin(), end = pPhotosPerName.cend(); it != end; ++it)
		{
			YString newIncompleteName(pTargetFolder);
			newIncompleteName << IO::Separator << it->first; // without index or extension
			const ExtendedPhoto::Vector& photos = it->second;

			unsigned int size = static_cast<unsigned int>(photos.size());

			if (size == 1u)
			{
				auto& photoPtr = photos[0];
				assert(!(!photoPtr));
				YString name(newIncompleteName);
				name << ".jpg";

				YString originalPath = photoPtr->originalPath();

				// Yuni doesn't seem to have any move function...
				if (!moveFile(logs, originalPath, name))
					return false;
			}
			else
			{
				auto nbDigits = numberOfDigits(size + 1u);
				enum
				{
					maximum = 5
				};

				typedef CString<maximum, false> TinyString;
				TinyString index;
				index.resize(nbDigits);

				if (nbDigits > maximum)
				{
					logs.error() << "The program isn't expected to deal with "
						<< static_cast<unsigned int>(Math::Power(10., maximum))
						<< " photos in the same minute. "
						<< "Just modify enum maximum in Private::PopulateDayFolder cpp file"
						<< " and recompile";
					return false;
				}

				for (unsigned int i = 0u; i < size; ++i)
				{
					auto& photoPtr = photos[i];
					assert(!(!photoPtr));

					YString name(newIncompleteName);
					index.fill('0');
					index.overwriteRight(TinyString(i + 1u));
					name << '_' << index << ".jpg";

					YString originalPath = photoPtr->originalPath();

					if (originalPath != name)
					{
						if (!moveFile(logs, originalPath, name))
							return false;
					}
				}
			}
		}

		return true;
	}

	// Yuni doesn't have any move function for directory; mimic it crudely
	bool PopulateDayFolder::moveFile(LoggingFacility& logs,
		const YString& origin, const YString& target) const
	{
		{
			YString content(origin);
			content << ';' << target << '\n';

			if (!IO::File::AppendContent(pSummaryFile, content))
			{
				logs.error() << "Invalid summary file: " << pSummaryFile;
				return false;
			}
		}

		if (IO::File::Copy(origin, target) != IO::errNone)
		{
			logs.error() << "Unable to copy " << origin	<< " to " << target;
			return false;
		}

		if (IO::File::Delete(origin) != IO::errNone)
		{
			logs.error() << "Unable to delete " << origin;
			return false;
		}

		return true;
	}



} // namespace Private
} // namespace PictStock
