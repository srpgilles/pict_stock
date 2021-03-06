#include "sort_new_photos_iterator.hpp"
#include "../../photo_directory/path_format.hpp"
#include "../../extended_photo/cameras.hpp"


# ifdef USE_BOOST_REGULAR_EXPR
namespace regexNS = boost;
# else
namespace regexNS = std;
# endif // USE_BOOST_REGULAR_EXPR


namespace PictStock
{
namespace SortNewPhotos
{
namespace Private
{

	namespace // anonymous
	{
		void RequestText(LoggingFacility& logs, const AnyString& folderName, const YString& dateInMemory)
		{
			logs.info() << "Currently going through folder " << folderName << "; do you want to "
				"associate a date to all the pictures inside this directory ?";
			logs.info() << "\t1 -> no (use the date inside Exif pictures)";
			logs.info() << "\t2 -> yes (specify the date under format YYYYMMDD in next prompt)";

			if (!dateInMemory.empty())
				logs.info() << "\t3 -> yes (use date currently in memory which is "
					<< dateInMemory << ")";
		}

		static const YString expression =
			YString('^')
			<< '(' << PictStock::ExtendedPhoto::Private::Year::Regex() << ')'
			<< '(' << PictStock::ExtendedPhoto::Private::Month::Regex() << ')'
			<< '(' << PictStock::ExtendedPhoto::Private::Day::Regex() << ')'
			<< '$';

		/*!
		** \brief Regular expression for date formatting
		**
		** Basically format is YYYY:MM:DD
		*/
		static const regexNS::regex RegexDateFormatting(expression.c_str());
	}

	using namespace Yuni;

	SortNewPhotosIterator::SortNewPhotosIterator(LoggingFacility& logs,
		const ExtendedPhoto::Cameras& cameras,
		const String& inputDirectory,
		const PhotoDirectory::PathFormat& pathFormat,
		bool doFolderManualDate)
		: logs(logs),
		  pPathFormat(pathFormat),
		  pDoFolderManualDate(doFolderManualDate),
		  pFolderLevel(0u),
		  pCurrentFolderManualLevel(static_cast<unsigned int>(-1)),
		  pCameras(cameras)
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
		proposeSetManualDate(rootFolder);
		return true;
	}

	SortNewPhotosIterator::Flow SortNewPhotosIterator::onBeginFolder(const String& folderName, const String&, const String&)
	{
		++pFolderLevel;
		proposeSetManualDate(folderName);
		return IO::flowContinue;
	}

	void SortNewPhotosIterator::proposeSetManualDate(const String& folderName)
	{
		// If the option is disabled, do nothing
		if (!pDoFolderManualDate)
			return;

		// If the current folder is already encompassed by a previous choice, do nothing
		if (pFolderLevel > pCurrentFolderManualLevel)
			return;

		{
			// First ask the user its choice
			RequestText(logs, folderName,  pCurrentFolderManualDate);
			std::string answer;

			while (std::cin >> answer && answer != "1" && answer != "2" && answer != "3")
				RequestText(logs, folderName,  pCurrentFolderManualDate);

			if (answer == "1")
				pCurrentFolderManualDate.clear();
			else if (answer == "2")
			{
				bool isValid = false;
				regexNS::cmatch match;

				do
				{
					logs.info() << "Please answer the date (under format YYYYMMDD)";
					std::cin >> answer;

					isValid = regex_search(answer.c_str(), match, RegexDateFormatting);
				} while (!isValid);

				pCurrentFolderManualDate = answer;
			}
		}

		{
			// Specify whether this choice propagates to subfolders or not
			std::string answer;
			do
			{
				logs.info() << "Does this choice also apply to subfolders (y/n)?";
				std::cin >> answer;

			} while (answer != "y" && answer != "n");

			if (answer == "y")
				pCurrentFolderManualLevel = pFolderLevel;
			else
				pCurrentFolderManualLevel = static_cast<unsigned int>(-1);

		}
	}

	void SortNewPhotosIterator::onEndFolder(const String&, const String&, const String&)
	{
		--pFolderLevel;
	}


	SortNewPhotosIterator::Flow SortNewPhotosIterator::onFile(const String&, const String& folder,
		const String& name, uint64 /*size*/)
	{
		// Only consider files which extension is JPG or jpg
		if (!ExtendedPhoto::isExtensionManaged(name))
			return IO::flowContinue;

		String fullName;
		fullName << folder << IO::Separator << name;
		ExtendedPhoto::ExtendedPhoto::Ptr photoPtr =
			new ExtendedPhoto::ExtendedPhoto(logs, pCameras, fullName);
		auto& photo = *photoPtr;

		if (photo.problem())
		{
			// TODO Handle the case in which a photo can't be processed
			logs.error() << "SKIP PHOTO " << fullName;
			return IO::flowContinue;
		}

		if (!pCurrentFolderManualDate.empty())
		{
			assert(pDoFolderManualDate);
			photo.modifyDate(pCurrentFolderManualDate);
		}

		ExtendedPhoto::PathInformations usefulInfos(logs, pCameras);
		pPathFormat.onlyUsefulFolderElements(usefulInfos, photo);
		pPicturesToProcess[usefulInfos].push_back(photoPtr);

		return IO::flowContinue;
	}

	void SortNewPhotosIterator::onTerminate()
	{ }


} // namespace Private
} // namespace SortNewPhotos
} // namespace PictStock

