#include "sort_new_photos_iterator.hpp"
#include "date_tools.hxx"

namespace SgPhoto
{
namespace Private
{
	namespace // anonymous
	{
		void RequestText(LoggingFacility& logs, const AnyString& folderName, const YString& dateInMemory)
		{
			logs.info() << "Currently going through folder " << folderName << "; do you want to "
				"associate a date to all the pictures inside this directory (subdirectories handled "
				"separately)?";
			logs.info() << "\t1 -> no (use the date inside Exif pictures)";
			logs.info() << "\t2 -> yes (specify the date under format YYYYMMDD in next prompt)";

			if (!dateInMemory.empty())
				logs.info() << "\t3 -> yes (use date currently in memory which is "
					<< dateInMemory << ")";
		}
	}


	using namespace Yuni;

	SortNewPhotosIterator::SortNewPhotosIterator(LoggingFacility& logs,
		const String& inputDirectory, bool doFolderManualDate)
		: logs(logs),
		  pDoFolderManualDate(doFolderManualDate)
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


	SortNewPhotosIterator::~SortNewPhotosIterator()
	{
		// For code robustness and to avoid corrupt vtable
		stop();
	}


	bool SortNewPhotosIterator::onStart(const String& rootFolder)
	{
		logs.debug() << " [+] " << rootFolder;
		return true;
	}

	SortNewPhotosIterator::Flow SortNewPhotosIterator::onBeginFolder(const String& filename, const String&, const String& name)
	{
		logs.debug() << " [+] " << name;

		if (pDoFolderManualDate)
		{
			RequestText(logs, filename,  pCurrentFolderManualDate);
			std::string answer;

			while (std::cin >> answer && answer != "1" && answer != "2" && answer != "3")
				RequestText(logs, filename,  pCurrentFolderManualDate);

			if (answer == "1")
				pCurrentFolderManualDate.clear();
			else if (answer == "2")
			{
				bool isValid = false;
				do
				{
					logs.info() << "Please answer the date (under format YYYYMMDD)";
					if (answer.size() != 8)
						continue;

					DateString buf(answer);
					isValid =  isValidStringDate(buf);

				} while (!isValid && std::cin >> answer);
			}
		}

		return IO::flowContinue;
	}

	void SortNewPhotosIterator::onEndFolder(const String&, const String&, const String&)
	{ }


	SortNewPhotosIterator::Flow SortNewPhotosIterator::onFile(const String&, const String& folder,
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
		ExtendedPhoto::Ptr photoPtr = new ExtendedPhoto(logs, fullName);

		if (photoPtr->problem())
		{
			// TODO Handle the case in which a photo can't be processed
			logs.error() << "SKIP PHOTO " << fullName;
			return IO::flowContinue;
		}

		if (!pCurrentFolderManualDate.empty())
		{
			assert(pDoFolderManualDate);
			pPicturesToProcess[pCurrentFolderManualDate].push_back(photoPtr);
		}
		else
			pPicturesToProcess[photoPtr->date()].push_back(photoPtr);

		return IO::flowContinue;
	}

	void SortNewPhotosIterator::onTerminate()
	{
		logs.debug() << "END\n";
	}




} // namespace Private
} // namespace SgPhoto

