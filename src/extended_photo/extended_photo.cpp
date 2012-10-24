#include "extended_photo.hpp"
#include "../database/database.hpp"

#include <iostream>
#include <iomanip>

using namespace Yuni;

namespace PictStock
{
namespace ExtendedPhoto
{

    ExtendedPhoto::ExtendedPhoto(LoggingFacility& logs, const Database::Database& database, const String& filename)
		: logs(logs),
		  pOriginalPath(filename),
          pPathInformations(new PathInformations(logs, database)),
		  pStatus(epFine),
          pDatabase(database)
	{
		// It is assumed files passed in parameters truly exists
		// (check should occur before call to the class)
		#ifndef NDEBUG
		if (!IO::Exists(filename))
		{
			logs.error() << "Photo " << filename << " doesn't exist";
			assert(false);
		}
		#endif // NDEBUG

		try
		{
			pImage = Exiv2::ImageFactory::open(filename.c_str());
			assert(pImage.get() != 0);
			pImage->readMetadata();

			if (!identifyPhotographer())
				logs.warning() << "Photographer not identified for photo " << filename;

			if (!extractDate())
				pStatus = epExiv2Problem;
		}
		catch (const std::exception& e)
		{
			// I do not use exception myself, so any exception stems from exiv2
			logs.error() << "Exception caught for photo " << filename << ": " << e.what();
			pStatus = epExiv2Problem;
		}
	}


	bool ExtendedPhoto::identifyPhotographer()
	{
		// Unfortunately, there is no generic exif tag to identify a camera, and
		// for some older model there is no serial number either (so we just have
		// the name of the model, which is good enough for our purposes due to the
		// maze of existing models)
		// So we're checking one after another the keywords used by the known cameras
        const auto& cameras = pDatabase.cameras();
        const auto& keywords = cameras.keywords();

        Database::Photographer::Ptr photographerPtr(nullptr);

		for (auto it = keywords.cbegin(), end = keywords.cend(); it != end; ++it)
		{
			const auto& currentKeyword = *it;

			// Check whether the current keyword may be found in the exif data. If not,
			// skip it and try next one
			YString value;
			if (!findExifKey(currentKeyword, value))
				continue;

			// Now check whether one of the values in the database match the value read
            if (!cameras.identifyPhotographer(currentKeyword, value, photographerPtr))
				continue;

			assert(!(!photographerPtr));
			assert(!(!pPathInformations));
			pPathInformations->setPhotographer(photographerPtr);

			return true;
		}

		return false;
	}

	bool ExtendedPhoto::extractDate()
	{
		// First try to read DateTimeOriginal, and if not possible have a shot at
		// DateTimeDigitized
		String dateRead;

		if (!findExifKey("Exif.Photo.DateTimeOriginal", dateRead))
		{
			if (!findExifKey("Exif.Photo.DateTimeDigitized", dateRead))
				return false;
		}

		Date date;

		bool ret = dateFromExif(logs, date, dateRead);

		if (!ret)
			return false;

		assert(!(!pPathInformations));
		pPathInformations->setDate(date);
		return true;
	}


	bool ExtendedPhoto::findExifKey(const std::string& key, String& value) const
	{
		const Exiv2::ExifData& exifData = pImage->exifData();

		try
		{
			Exiv2::ExifKey exifKey(key);
			Exiv2::ExifData::const_iterator citer = exifData.findKey(exifKey);

			if (citer != exifData.end())
			{
				value = citer->value().toString();
				value.trim();
				return true;
			}

			return false;
		}
		catch (...)
		{
			return false;
		}
	}

	bool ExtendedPhoto::printExifData(std::ostream& out) const
	{
		const Exiv2::ExifData& exifData = pImage->exifData();

		if (exifData.empty())
			return false;

		for (auto i = exifData.begin(), end = exifData.end(); i != end; ++i)
		{
			out << std::setw(44) << std::setfill(' ') << std::left << i->key() << " " << "0x"
					<< std::setw(4) << std::setfill('0') << std::right << std::hex << i->tag() << " "
					<< std::setw(9) << std::setfill(' ') << std::left << i->typeName() << " " << std::dec
					<< std::setw(3) << std::setfill(' ') << std::right << i->count() << "  " << std::dec
					<< i->value() << '\n';
		}

		return true;
	}

	bool operator==(const ExtendedPhoto& photo1, const ExtendedPhoto& photo2)
	{
		{
			// First compare the sizes: if not equal result is false!
			uint64 size[2];
			IO::File::Size(photo1.pOriginalPath, size[0]);
			IO::File::Size(photo2.pOriginalPath, size[1]);

			if (size[0] != size[1])
				return false;
		}

		{
			// Then compare exif. Other datatype are currently ignored
			const auto& exifData1 = photo1.pImage->exifData();

			YString value1, value2;

			for (auto i = exifData1.begin(), end = exifData1.end(); i != end; ++i)
			{
				auto key = i->key();
				value1.clear();
				value2.clear();

				assert(photo1.findExifKey(key, value1));

				if (!photo2.findExifKey(key, value2))
					return false;

				if (value1 != value2)
					return false;
			}
		}

		return true;
	}


	bool ExtendedPhoto::modifyDate(const Yuni::CString<8, false>& newDate)
	{
		assert(newDate.size() == 8u);
		assert(!(!pPathInformations));
		auto& pathInformations = *pPathInformations;


		{
			// First modify path_informations date
			pathInformations.changeDate(newDate);
		}

		Exiv2::ExifData& exifData = pImage->exifData();

		auto newExifDate = dateToExif(pathInformations.date());

		CString<150, false> comment("The date has been modified manually; ");
		{
			// Read current value and prepare comment

			String currentDate;
			if (findExifKey("Exif.Photo.DateTimeOriginal", currentDate))
				comment << "previously Exif.Photo.DateTimeOriginal = " << currentDate;
			else if (findExifKey("Exif.Photo.DateTimeDigitized", currentDate))
				comment << "previously Exif.Photo.DateTimeDigitized = " << currentDate;
			else
				comment << "no value was set previously";
		}

		try
		{
			exifData["Exif.Photo.DateTimeOriginal"] = newExifDate.c_str();
			exifData["Exif.Photo.DateTimeDigitized"] = newExifDate.c_str();
			exifData["Exif.Photo.UserComment"] = comment.c_str();
			pImage->writeMetadata();
			return true;
		}
		catch (const std::exception& e)
		{
			logs.error() << e.what();
			return false;
		}
		catch (...)
		{
			logs.error() << "Non-standard exception caught";
			return false;
		}

	}

	bool isExtensionManaged(const YString& file)
	{
		// Check whether it is a valid jpg file
		String extension;
		IO::ExtractExtension(extension, file, false, false);

		// Only JPG files managed at the moment
		return (extension.toUpper() == "JPG");
	}

} // namespace ExtendedPhoto
} // namespace PictStock

