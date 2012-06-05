#ifndef DATE_TOOLS_HXX_
# define DATE_TOOLS_HXX_

namespace PictStock
{
	typedef Yuni::CString<8, false> DateString;

	namespace // anonymous
	{

		/*!
		 * \brief Analyze a part of the subfolder and decide whether this
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

		struct YearTraits
		{
			static const unsigned int Min = 1900u;
			static const unsigned int Max = 2100u;
		};

		struct MonthTraits
		{
			static const unsigned int Min = 1u;
			static const unsigned int Max = 12u;
			static const char Letter = 'M';
		};

		struct DayTraits
		{
			static const unsigned int Min = 1u;
			static const unsigned int Max = 31u;
			static const char Letter = 'J';
		};


		/*!
		 * \brief Same analyze is performed for month or day; so this function is called
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
			Yuni::CString<4, false> strBuf(a, b);

			return isNumericDate(folderDate, strBuf, DateTraits::Min, DateTraits::Max);
		}


		template<class DateTraits>
		bool isValidStringDateHelper(const DateString& string,
			unsigned int indexIteratorBegin, unsigned int indexIteratorEnd)
		{
			auto a = string.utf8begin() + indexIteratorBegin;
			auto b = string.utf8begin() + indexIteratorEnd;

			Yuni::CString<4, false> subpart(a, b);

			unsigned int numericValue;
			numericValue = subpart.to<unsigned int>();

			return !(numericValue < DateTraits::Min || numericValue > DateTraits::Max);
		}



		/*!
		 *  \brief Check whether a string follow the scheme YYYYMMDD
		 */
		bool isValidStringDate(const DateString& string)
		{
			if (string.size() != 8)
				return false;

			if (!isValidStringDateHelper<YearTraits>(string, 0, 4))
				return false;

			if (!isValidStringDateHelper<MonthTraits>(string, 4, 6))
				return false;

			if (!isValidStringDateHelper<DayTraits>(string, 6, 8))
				return false;

			return true;

		}


	} // namespace anonymous

} // namespace PictStock


#endif /* DATE_TOOLS_HXX_ */
