#include "photo_directory_iterator.hpp"
#include "path_format.hpp"

using namespace Yuni;

namespace PictStock
{
namespace Private
{


	PhotoDirectoryIterator::PhotoDirectoryIterator(LoggingFacility& logs, const PathFormat& pathFormat)
		: logs(logs),
		  pPathFormat(pathFormat)
	{ }


	PhotoDirectoryIterator::~PhotoDirectoryIterator()
	{
		// For code robustness and to avoid corrupt vtable
		stop();
	}


	void PhotoDirectoryIterator::validDirectories(ValidDirectoriesType& list)
	{
		list = pValidDirectories;
	}


	bool PhotoDirectoryIterator::onStart(const String& rootFolder)
	{
		logs.info() << " [+] " << rootFolder;
		pFolderCount = 0;
		pValidFolderCount = 0;

		return true;
	}


	PhotoDirectoryIterator::Flow PhotoDirectoryIterator::onBeginFolder(const String& completePath,
		const String&, const String& /*name*/)
	{
		if (checkValidity(completePath))
			++pValidFolderCount;

		++pFolderCount;
		
		return IO::flowContinue;
	}

	void PhotoDirectoryIterator::onEndFolder(const String&, const String&, const String&)
	{ }

	PhotoDirectoryIterator::Flow PhotoDirectoryIterator::onFile(const String&,
		const String& /*folder*/, const String& /*name*/, uint64 /*size*/)
	{
		return IO::flowContinue;
	}

	void PhotoDirectoryIterator::onTerminate()
	{
		logs.info() << pValidFolderCount << " / " << pFolderCount
			<< " folder(s) are valid";
	}


	bool PhotoDirectoryIterator::checkValidity(const String& directory)
	{
		std::map<Traits::Element::Ptr, Yuni::CString<10, false> > elements;

		if (pPathFormat.doFolderMatch(directory, elements))
		{
			auto foo = pPathFormat.folderContent();

			//RelevantInformations infos =
			//pValidDirectories;
			//pPathFormat.determineFolderKey(buf, )
			//pValidDirectories.insert(make_pair(pPathFormat.));
		}

		// TODO Complete!


		return true;

//		// Split the path in subfolders to see if they are legit directories
//		std::vector<String> subPath;
//		directory.split(subPath, IO::Separator);
//
//		String folderDate;
//
//		auto it = subPath.cbegin();
//		auto end = subPath.cend();
//
//
//		// Here we have found nothing so far, and are looking
//		// for year element. Year element is just an integer in
//		// complete form
//		{
//			bool found = false;
//			for (; !found && it != end ; ++it)
//				found = isNumericDate(folderDate, *it, YearTraits::Min, YearTraits::Max);
//
//			if (it == end)
//				return false;
//		}
//
//		// Now we want to complete the month. Current accepted format is Mxx or
//		// Mxx_* (but not Mxx-*; M04-05 is for instance not valid and should be
//		// completed by subfolders M04 and M05 inside)
//		// As of this writing, C++11 regexp not supported by g++, so I'll do
//		// it more manually (this should be changed for more flexibility later)
//		{
//			bool found = false;
//
//			for (; !found && it != end ; ++it)
//				found = monthOrDayHelper<MonthTraits>(folderDate, *it);
//
//			if (it == end)
//				return false;
//		}
//
//		// Do the same stuff for day
//		{
//			bool found = false;
//
//			for (; !found && it != end ; ++it)
//				found = monthOrDayHelper<DayTraits>(folderDate, *it);
//
//			if (found)
//				pValidDirectories[folderDate].push_back(directory);
//
//			return found;
//		}
	}


} // namespace Private
} // namespace PictStock
