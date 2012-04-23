#include <fstream>
#include "sort_new_photos.hpp"

using namespace Yuni;

namespace SgPhoto
{


	SortNewPhotos::SortNewPhotos(LoggingFacility& logs, const String& inputDirectory,
		PhotoDirectory& photoDirectory)
		: logs(logs),
		  pPhotoDirectory(photoDirectory),
		  pInputDirectory(inputDirectory)
	{
		if (!IO::Directory::Exists(inputDirectory))
		{
			logs.fatal() << "Error: input directory " << inputDirectory
				<< " doesn't exists\n";
			exit(-1); // TODO proper error handling to foresee
		}

		{
			// Now we want to iterate through the new files to add in the photo directory
			add(inputDirectory);
			start();
			wait();
		}


	}


	SortNewPhotos::~SortNewPhotos()
	{
		// For code robustness and to avoid corrupt vtable
		stop();
	}


	bool SortNewPhotos::onStart(const String& rootFolder)
	{
		logs.debug() << " [+] " << rootFolder;
		return true;
	}

	SortNewPhotos::Flow SortNewPhotos::onBeginFolder(const String&, const String&, const String& name)
	{
		logs.debug() << " [+] " << name;

		// Here reinit choices that might be folder related (impose date in photos, etc...)

		return IO::flowContinue;
	}

	void SortNewPhotos::onEndFolder(const String&, const String&, const String&)
	{ }


	SortNewPhotos::Flow SortNewPhotos::onFile(const String&, const String& folder,
		const String& name, uint64 /*size*/)
	{
		// Only consider files which extension is JPG or jpg
		{
			String ext;
			IO::ExtractExtension(ext, name);

			if (ext.toLower() != ".jpg")
			{
				logs.debug() << "SKIP not jpeg file " << name;
				return IO::flowContinue;
			}
		}

		String fullName;
		fullName << folder << IO::Separator << name;
		ExtendedPhoto myPhoto(logs, fullName);

		{
			if (myPhoto.problem())
			{
				// TODO Handle the case in which a photo can't be processed
				logs.debug() << "SKIP PHOTO " << fullName;

				return IO::flowContinue;
			}
		}

		{
			// Check whether the date of the photo already exists or not
			auto photoDate = myPhoto.date();
			std::list<YString> folders;

			if (!pPhotoDirectory.findDate(photoDate, folders))
			{
				// If the correct folder doesn't exist, just create it and move the photo there

			}

		}


		return IO::flowContinue;
	}

	void SortNewPhotos::onTerminate()
	{
		logs.debug() << "END\n";
	}





} // namespace SgPhoto
