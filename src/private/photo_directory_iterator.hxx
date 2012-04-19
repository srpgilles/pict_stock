#ifndef PHOTO_DIRECTORY_ITERATOR_HXX_
# define PHOTO_DIRECTORY_ITERATOR_HXX_


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
		bool isNumericDate(YString& folderDate, const YString& toAnalyse,
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


		template<class LanguageT>
		struct DayTraits
		{
			static const unsigned int Min = 1;
			static const unsigned int Max = 31;
			static const char Letter = LanguageT::DayLetter;
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
		bool monthOrDayHelper(YString& folderDate, const YString& subfolder)
		{
			if (!subfolder.startsWith(DateTraits::Letter))
				return false;

			if (subfolder.size() > 3 && subfolder[3] == '-')
				return false;

			YString::const_utf8iterator a = subfolder.utf8begin() + 1;
			YString::const_utf8iterator b = subfolder.utf8begin() + 3;
			YString strBuf(a, b);

			return isNumericDate(folderDate, strBuf, DateTraits::Min, DateTraits::Max);
		}


	} // namespace anonymous


	template<class LanguageT>
	PhotoDirectoryIterator<LanguageT>::PhotoDirectoryIterator(LoggingFacility& logs)
		: logs(logs)
	{ }


	template<class LanguageT>
	PhotoDirectoryIterator<LanguageT>::~PhotoDirectoryIterator()
	{
		// For code robustness and to avoid corrupt vtable
		stop();
	}

	template<class LanguageT>
	void PhotoDirectoryIterator<LanguageT>::validDirectories(ValidDirectoriesType& list)
	{
		list = pValidDirectories;
	}

	template<class LanguageT>
	bool PhotoDirectoryIterator<LanguageT>::onStart(const YString& rootFolder)
	{
		logs.info() << " [+] " << rootFolder;
		pCounter = 1;
		pFolderCount = 0;
		pValidFolderCount = 0;

		return true;
	}

	template<class LanguageT>
	Yuni::IO::Flow PhotoDirectoryIterator<LanguageT>::onBeginFolder(const YString& completePath,
		const YString&, const YString& /*name*/)
	{
		// Valid directories must include at least three components (for year, month, day)
		if (pCounter++ < 3)
			return Yuni::IO::flowContinue;

		if (checkValidity(completePath))
			++pValidFolderCount;

		++pFolderCount;
		return Yuni::IO::flowContinue;
	}

	template<class LanguageT>
	void PhotoDirectoryIterator<LanguageT>::onEndFolder(const YString&, const YString&, const YString&)
	{
		--pCounter;
	}

	template<class LanguageT>
	Yuni::IO::Flow PhotoDirectoryIterator<LanguageT>::onFile(const YString&,
		const YString& /*folder*/, const YString& /*name*/, Yuni::uint64 /*size*/)
	{
		return Yuni::IO::flowContinue;
	}

	template<class LanguageT>
	void PhotoDirectoryIterator<LanguageT>::onTerminate()
	{
		logs.info() << pValidFolderCount << " / " << pFolderCount
			<< " folder(s) are valid";
	}

	template<class LanguageT>
	bool PhotoDirectoryIterator<LanguageT>::checkValidity(const YString& directory)
	{
		// Split the path in subfolders to see if they are legit directories
		std::vector<YString> subPath;
		directory.split(subPath, Yuni::IO::Separator);

		YString folderDate;

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
				found = monthOrDayHelper<DayTraits<LanguageT> >(folderDate, *it);

			return found;
		}
	}


} // namespace Private
} // namespace SgPhoto

#endif /* PHOTO_DIRECTORY_ITERATOR_HXX_ */
