#include <yuni/io/directory/info.h>
#include <yuni/io/directory/system.h>
#include <yuni/datetime/timestamp.h>
#include <yuni/core/math.h>
#include <fstream>
#include "populate_day_folder.hpp"

using namespace Yuni;

namespace SgPhoto
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


	PopulateDayFolder::PopulateDayFolder(LoggingFacility& logs, const YString& targetFolder,
		const DateString& targetDate, ExtendedPhoto::Vector& newPhotos)
		: logs(logs),
		  pTargetFolder(targetFolder),
		  pTargetDate(targetDate),
		  pNewPhotos(newPhotos)
	{ }

	PopulateDayFolder::~PopulateDayFolder()
	{ }


	bool PopulateDayFolder::proceed()
	{
		// Modify if necessary the date in exif
		if (!enforceDateInNewPhotos())
			return false;

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


	bool PopulateDayFolder::enforceDateInNewPhotos()
	{
		for (auto it = pNewPhotos.begin(), end = pNewPhotos.end(); it != end; ++it)
		{
			ExtendedPhoto::Ptr photoPtr = *it;
			assert(!(!photoPtr));
			ExtendedPhoto& photo = *photoPtr;

			if (photo.date() != pTargetDate)
			{
				if (!photo.modifyDate(pTargetDate))
					return false;
			}
		}

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
			{
				logs.error() << "Unable to extract extension of file " << file;
				return false;
			}

			if (ext.toLower() != ".jpg")
				continue;

			ExtendedPhoto::Ptr photoPtr = new ExtendedPhoto(logs, file);

			YString newName;
			photoPtr->newNameWithoutExtension(newName);
			pPhotosPerName[newName].push_back(photoPtr);
		}

		return true;
	}

	void PopulateDayFolder::insertNewPhotos()
	{
		// TODO Check new photos aren't redundant with pre-existing ones
		for (auto it = pNewPhotos.cbegin(), end = pNewPhotos.cend(); it != end; ++it)
		{
			ExtendedPhoto::Ptr photoPtr = *it;
			assert(!(!photoPtr));
			YString newName;
			ExtendedPhoto& photo = *photoPtr;
			photo.newNameWithoutExtension(newName);

			{
				ExtendedPhoto::Vector& allPhotos = pPhotosPerName[newName];
				unsigned int size = static_cast<unsigned int>(allPhotos.size());

				bool doPhotoAlreadyExist = false;
				for (unsigned int i = 0; !doPhotoAlreadyExist && i < size; ++i)
				{
					if (*allPhotos[i] == photo)
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

				if (IO::File::Copy(photoPtr->originalPath(), name) != IO::errNone)
				{
					logs.error() << "Unable to copy " << photoPtr->originalPath()
						<< " to " << name;
					return false;
				}
			}
			else
			{
				auto nbDigits = numberOfDigits(size + 1u); // + 1u because we begin at 1 and not 0 the index

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
						if (IO::File::Copy(originalPath, name) != IO::errNone)
						{
							logs.error() << "Unable to copy " << originalPath
								<< " to " << name;
							return false;
						}
					}
				}
			}
		}

		return true;
	}


} // namespace Private
} // namespace SgPhoto
