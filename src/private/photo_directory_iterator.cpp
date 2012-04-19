#include "photo_directory_iterator.hpp"

using namespace Yuni;

namespace SgPhoto
{
namespace Private
{
	namespace
	{
		/*!
		 * \brief Analyse a part of the subfolder and decide whether this
		 * is a valid date or not
		 *
		 * For instance, month must be included between 1 and 12
		 *
		 * @param[in, out] folderDate Date in format YYYYMMDD. In construction when
		 * this function is called (will be called thrice: once for each date constituent)
		 * @param toAnalyse Part of the subfolder name that is to be transformed
		 * into a numerical figure. For instance 03 in case of subfolder M03__Foo
		 * @param min Minimal acceptable value. 1 for month for instance
		 * @param max Maximal acceptable value. 12 for month for instance
		 * @return
		 */
		bool isNumericDate(String& folderDate, const String& toAnalyse,
			unsigned int min, unsigned max)
		{
			unsigned int buf;
			if (toAnalyse.to(buf) && buf >= min && buf <= max)
			{
				folderDate << toAnalyse;
				return true;
			}

			return false;

		}

		struct MonthTraits
		{
			static const unsigned int Min = 1;
			static const unsigned int Max = 12;
			static const char Letter = 'M';
		};

		struct DayTraits
		{
			static const unsigned int Min = 1;
			static const unsigned int Max = 31;
			static const char Letter = 'J';
		};


		/*!
		 * \brief Same analyse is performed for month or day; so this function is called
		 *
		 * \tparam DateTraits MonthTraits or DayTraits
		 * \param[in, out] folderDate Date in format YYYYMMDD. In construction when
		 * this function is called (will be called thrice: once for each date constituent)
		 * \param[in] subfolder Part of the path being under study. For instance M03-04__Foo
		 */
		template<class DateTraits>
		bool monthOrDayHelper(String& folderDate, const String& subfolder)
		{
			if (!subfolder.startsWith(DateTraits::Letter))
				return false;

			if (subfolder.size() > 3 && subfolder[3] == '-')
				return false;

			String::const_utf8iterator a = subfolder.utf8begin() + 1;
			String::const_utf8iterator b = subfolder.utf8begin() + 3;
			String strBuf(a, b);

			return isNumericDate(folderDate, strBuf, DateTraits::Min, DateTraits::Max);
		}


	} // namespace anonymous



	PhotoDirectoryIterator::PhotoDirectoryIterator(LoggingFacility& logs)
		: logs(logs)
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
		pCounter = 1;
		pFolderCount = 0;
		pValidFolderCount = 0;

		return true;
	}


	PhotoDirectoryIterator::Flow PhotoDirectoryIterator::onBeginFolder(const String& completePath,
		const String&, const String& /*name*/)
	{
		// Valid directories must include at least three components (for year, month, day)
		if (pCounter++ < 3)
			return IO::flowContinue;

		if (checkValidity(completePath))
			++pValidFolderCount;

		++pFolderCount;
		return IO::flowContinue;
	}

	void PhotoDirectoryIterator::onEndFolder(const String&, const String&, const String&)
	{
		--pCounter;
	}

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
		// Split the path in subfolders to see if they are legit directories
		std::vector<String> subPath;
		directory.split(subPath, IO::Separator);

		String folderDate;

		auto it = subPath.cbegin();
		auto end = subPath.cend();


		// Here we have found nothing so far, and are looking
		// for year element. Year element is just an integer in
		// complete form
		{
			bool found = false;
			for (; !found && it != end ; ++it)
				found = isNumericDate(folderDate, *it, 1900u, 2100u);

			if (it == end)
				return false;
		}

		// Now we want to complete the month. Current accepted format is Mxx or
		// Mxx_* (but not Mxx-*; M04-05 is for instance not valid and should be
		// completed by subfolders M04 and M05 inside)
		// As of this writing, C++11 regexp not supported by g++, so I'll do
		// it more manually (this should be changed for more flexibility later)
		{
			bool found = false;

			for (; !found && it != end ; ++it)
				found = monthOrDayHelper<MonthTraits>(folderDate, *it);

			if (it == end)
				return false;
		}

		// Do the same stuff for day
		{
			bool found = false;

			for (; !found && it != end ; ++it)
				found = monthOrDayHelper<DayTraits>(folderDate, *it);

			return found;
		}
	}


} // namespace Private
} // namespace SgPhoto
