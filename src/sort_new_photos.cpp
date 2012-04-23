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
//


//		{
//			// Debug check
//			std::ofstream out("/tmp/checkDirectories.txt");
//			printValidDirectories(out);
//			out.close();
//		}
//
//		{
//			// Now we want to iterate through the new files to add in the photo directory
//			add(inputDirectory);
//			start();
//			wait();
//		}



		//PhotoDir.validDirectories()

	/*	IterateThroughInputDirectory zorglub;
		zorglub.add(inputDirectory);

		zorglub.start();
		zorglub.wait();*/


		// Io::ExtractFilePath()

	}


	SortNewPhotos::~SortNewPhotos()
	{
		// For code robustness and to avoid corrupt vtable
		stop();
	}


	bool SortNewPhotos::onStart(const String& rootFolder)
	{
		std::cout << " [+] " << rootFolder << std::endl;
		return true;
	}

	SortNewPhotos::Flow SortNewPhotos::onBeginFolder(const String&, const String&, const String& name)
	{
		std::cout << " [+] " << name << std::endl;

		// Here reinit choicesd that might be folder related (impose date in photos, etc...)

		return IO::flowContinue;
	}

	void SortNewPhotos::onEndFolder(const String&, const String&, const String&)
	{
	}


	SortNewPhotos::Flow SortNewPhotos::onFile(const String&, const String& folder,
		const String& name, uint64 size)
	{
		// Only consider files which extension is JPG or jpg
		String ext;
		IO::ExtractExtension(ext, name);

		if (ext.toLower() != ".jpg")
		{
			std::cout << "SKIP " << name << "\n";
			return IO::flowContinue;
		}


//		std::cout << "  -  " << name << " (" << size << " bytes)" << std::endl;
		String fullName;
		fullName << folder << IO::Separator << name;
		ExtendedPhoto myPhoto(logs, fullName);

		if (myPhoto.problem())
		{
			// TODO Handle the case in which a photo can't be processed
			std::cout << "SKIP PHOTO " << fullName << "\n";

			return IO::flowContinue;
		}

		// If an automatic choice has already been programmed, simply put the file there
		// without further question
		{
			YString photoDate = myPhoto.date();

			auto it = pAutomaticChoice.find(photoDate);

			if (it != pAutomaticChoice.end())
			{
				// Simply move the photo to the chosen directory, and rename it
				// use method movePhoto();


			}




		}


		//pValidDirectories



		return IO::flowContinue;
	}

	void SortNewPhotos::onTerminate()
	{
		std::cout << "END\n";
	}


	void SortNewPhotos::printValidDirectories(std::ostream& out) const
	{
		for (auto it = pValidDirectories.begin(), end = pValidDirectories.end(); it != end; ++it)
		{
			out << it->first << "\n";
			std::list<String> mylist = it->second;

			for (auto dirIt = mylist.begin(), dirEnd = mylist.end(); dirIt != dirEnd; ++dirIt)
				out << "\t" << *dirIt << "\n";
		}
	}




} // namespace SgPhoto
