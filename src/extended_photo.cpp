#include "extended_photo.hpp"
#include <iostream>
#include <iomanip>

using namespace Yuni;

namespace PictStock
{

	namespace
	{

		/*!
		 ** Init the list of photographers
		 **
		 ** Quite crude at the moment; should not be written in hard but read
		 ** instead from a file or a database
		 **
		 ** TODO Temporary; should be handled through a sqlite database
		 */

		static Photographer::List initPhotographers()
		{
			Photographer::List ret;

			{
				// Claire et Sébastien
				std::multimap<std::string, String> cameras;
				cameras.insert(std::make_pair("Exif.Canon.SerialNumber", "2280522782"));
				cameras.insert(std::make_pair("Exif.Image.Model", "FinePix A350"));
				cameras.insert(std::make_pair("Exif.Image.Model", "FinePix E500"));

				Photographer::Ptr newEntry = new Photographer("Claire et Sébastien", "CSG", cameras);

				ret.push_back(newEntry);
			}

			{
				// Aurelien
				std::multimap<std::string, String> cameras;
				cameras.insert(std::make_pair("Exif.Canon.SerialNumber", "430125393"));
				cameras.insert(std::make_pair("Exif.Image.Model", "FinePix S5000"));

				Photographer::Ptr newEntry = new Photographer("Aurélien", "AG", cameras);

				ret.push_back(newEntry);
			}

			{
				// Annie et Christian
				std::multimap<std::string, String> cameras;
				cameras.insert(std::make_pair("Exif.Image.Model", "DSC-W70"));

				Photographer::Ptr newEntry = new Photographer("Annie et Christian", "AGC", cameras);

				ret.push_back(newEntry);
			}

			{
				// Papy Mamie
				std::multimap<std::string, String> cameras;
				cameras.insert(std::make_pair("Exif.Image.Model", "DMC-FS4"));

				Photographer::Ptr newEntry = new Photographer("Papy et Mamie", "PMS", cameras);

				ret.push_back(newEntry);
			}

			{
				// René
				std::multimap<std::string, String> cameras;
				cameras.insert(std::make_pair("ExifPhoto.__NikonSerialNumbers", "4045196"));

				Photographer::Ptr newEntry = new Photographer("René", "RW", cameras);

				ret.push_back(newEntry);
			}

			{
				// Laurent et Céline
				std::multimap<std::string, String> cameras;
				cameras.insert(std::make_pair("Exif.Canon.SerialNumber", "1531001946"));

				Photographer::Ptr newEntry = new Photographer("Laurent et Céline", "LCSC", cameras);

				ret.push_back(newEntry);
			}

			{
				// Parents de Thuy
				std::multimap<std::string, String> cameras;
				cameras.insert(std::make_pair("Exif.Image.Model", "FinePix S6500fd"));
				cameras.insert(std::make_pair("Exif.Image.Model", "DMC-TZ1"));
				cameras.insert(
						std::make_pair("Exif.Image.Model",
								"SAMSUNG ST550 / SAMSUNG ST560 / VLUU ST550 / SAMSUNG TL225"));
				cameras.insert(std::make_pair("Exif.Nikon3.SerialNumber", "6100268"));
				cameras.insert(std::make_pair("Exif.Image.ImageDescription", "SONY DSC"));

				Photographer::Ptr newEntry = new Photographer("Parents de Thuy", "T", cameras);

				ret.push_back(newEntry);
			}

			return ret;

		}
	} // anonymous namespace

	const Photographer::List ExtendedPhoto::pPhotographers = initPhotographers();

	ExtendedPhoto::ExtendedPhoto(LoggingFacility& logs, const String& filename)
			: logs(logs), pOriginalPath(filename), pPhotographer(nullptr), pStatus(epFine)
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
		// So we have to proceed by trial and error

		for (auto it = pPhotographers.cbegin(), end = pPhotographers.cend(); it != end; ++it)
		{
			assert(!(!(*it)));

			const auto& cameras = (*it)->exifData();

			String value;

			for (auto itMap = cameras.cbegin(), endMap = cameras.cend(); itMap != endMap; ++itMap)
			{
				if (findExifKey(itMap->first, value))
				{
					if (value == itMap->second)
					{
						pPhotographer = *it;
						return true;
					}
				}
			}
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

		std::vector<int> dateAsVect;

		// Now parse this date to extract year, month, day, hour, minute, second
		dateRead.split(dateAsVect, " :");

		if (dateAsVect.size() != 6)
		{
			logs.fatal() << "Date is expected to follow format %y:%m:%d %h:%m:%s, "
					"which was not the case of " << dateRead << "\n";
			return false;
		}

		typedef CString<4, false> TinyString;

		{
			// Proceed to extract the date and write in format YYYYMMDD
			pStringDate.resize(8);
			pStringDate.fill('0');

			pStringDate.overwrite(TinyString(dateAsVect[0]));
			pStringDate.overwriteRight(2, TinyString(dateAsVect[1]));
			pStringDate.overwriteRight(TinyString(dateAsVect[2]));
		}

		{
			// Same stuff for hour and minutes (seconds are dropped)
			pStringTime.resize(4);
			pStringTime.fill('0');
			pStringTime.overwriteRight(2, TinyString(dateAsVect[3]));
			pStringTime.overwriteRight(TinyString(dateAsVect[4]));
		}

		return true;
	}

	bool ExtendedPhoto::findExifKey(const std::string& key, String& value) const
	{
		const Exiv2::ExifData& exifData = pImage->exifData();

		try
		{
			Exiv2::ExifData::const_iterator citer = exifData.findKey(Exiv2::ExifKey(key));

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

	void ExtendedPhoto::newNameWithoutExtension(YString& name) const
	{
		name.clear() << "Photo";

		if (pStringTime != "9999") // 9999 is the value when the date has been manually set
			name << '_' << pStringTime;

		if (!pPhotographer)
			name << '_' << "UNK";
		else
			name << '_' << pPhotographer->abbr();
	}

	bool ExtendedPhoto::modifyDate(const DateString& newDate)
	{
		assert(newDate.size() == 8u);
		Exiv2::ExifData& exifData = pImage->exifData();

		YString newExifDate(newDate, 0, 4);

		{
			// Prepare new date to put into the exif file
			newExifDate << ':';
			newExifDate.append(newDate, 2, 4);
			newExifDate << ':';
			newExifDate.append(newDate, 2, 6);
			newExifDate << " 99:99:99"; // fake hour!
		}

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

} // namespace PictStock

