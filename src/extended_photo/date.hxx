#ifndef DATE_HXX_
# define DATE_HXX_


namespace PictStock
{
namespace ExtendedPhoto
{

	namespace
	{
		/*!
		** \brief Function in charge of converting date element into
		** proper struct tm informations
		**
		** For instance, for year 2012 would become 112 (as struct tm
		** elements begin at 1900 for the year)
		**
		** \tparam An element of DateTuple
		**
		** \param[in] value Value before conversion (for instance 2012 for year)
		*/
		template<class T>
		void toCTimeInformations(struct tm& out, int value);

		template<>
		inline void toCTimeInformations<Private::Year>(struct tm& out, int value)
		{
			out.tm_year = value - 1900;
		}


		template<>
		inline void toCTimeInformations<Private::Month>(struct tm& out, int value)
		{
			assert(value > 0 && value <= 12);
			out.tm_mon = --value;
		}

		template<>
		inline void toCTimeInformations<Private::Day>(struct tm& out, int value)
		{
			assert(value >= 1 && value <= 31);
			out.tm_mday = value;
		}

		template<>
		inline void toCTimeInformations<Private::Hour>(struct tm& out, int value)
		{
			assert(value >= 0 && value <= 23);
			out.tm_hour = value;
		}

		template<>
		inline void toCTimeInformations<Private::Minute>(struct tm& out, int value)
		{
			assert(value >= 0 && value <= 59);
			out.tm_min = value;

		}

		template<>
		inline void toCTimeInformations<Private::Second>(struct tm& out, int value)
		{
			assert(value >= 0 && value < 60);

			/*# ifdef YUNI_OS_WINDOWS
			long timezone;
			_get_timezone(&timezone);
			# endif*/

			out.tm_sec = value;// + static_cast<int>(timezone);
		}

	}



	/*!
	** \brief Helper to perform recursively all required conversions
	**
	** \param[in] in Array that contains original values that must be converted
	** to fit inside tm object. For instance, in[Element::year] = 2012 yields
	** 112 in pData structure. If -1, pIsElementPresent is put as false
	**
	** \tparam An element of DateTuple
	*/
	template<std::size_t I>
	typename std::enable_if<I == std::tuple_size<DateTuple>::value, void>::type
		Date::conversionHelper(const std::array<int, std::tuple_size<DateTuple>::value>& /*in*/)
	{ }


	template<std::size_t I>
	typename std::enable_if<I < std::tuple_size<DateTuple>::value, void>::type
		Date::conversionHelper(const std::array<int, std::tuple_size<DateTuple>::value>& in)
	{
		if (pIsElementPresent[I])
		{
			typedef typename std::tuple_element<I, DateTuple>::type type;
			toCTimeInformations<type>(pData, in[I]);
		}

		conversionHelper<I+1>(in);
	}





	template<class StringT>
	bool Date::print(StringT& out) const
	{
		assert(out.empty());

		if (pIsElementPresent.none())
		{
			out << "Empty!" << '\n';
			return true;
		}

		YString format;


        YUNI_STATIC_ASSERT((GenericTools::Tuple::IndexOf<Private::Year, DateTuple>::value >= 0),
			YearNotFoundInTuple);
        YUNI_STATIC_ASSERT((GenericTools::Tuple::IndexOf<Private::Month, DateTuple>::value >= 0),
			MonthNotFoundInTuple);
        YUNI_STATIC_ASSERT((GenericTools::Tuple::IndexOf<Private::Day, DateTuple>::value >= 0),
			DayNotFoundInTuple);
        YUNI_STATIC_ASSERT((GenericTools::Tuple::IndexOf<Private::Hour, DateTuple>::value >= 0),
			HourNotFoundInTuple);
        YUNI_STATIC_ASSERT((GenericTools::Tuple::IndexOf<Private::Minute, DateTuple>::value >= 0),
			MinuteNotFoundInTuple);
        YUNI_STATIC_ASSERT((GenericTools::Tuple::IndexOf<Private::Second, DateTuple>::value >= 0),
			SecondNotFoundInTuple);


        if (pIsElementPresent[GenericTools::Tuple::IndexOf<Private::Year, DateTuple>::value])
			format << "Year = %Y\n";

        if (pIsElementPresent[GenericTools::Tuple::IndexOf<Private::Month, DateTuple>::value])
			format << "Month = %m\n";

        if (pIsElementPresent[GenericTools::Tuple::IndexOf<Private::Day, DateTuple>::value])
			format << "Day = %d\n";

        if (pIsElementPresent[GenericTools::Tuple::IndexOf<Private::Hour, DateTuple>::value])
			format << "Hour = %H\n";

        if (pIsElementPresent[GenericTools::Tuple::IndexOf<Private::Minute, DateTuple>::value])
			format << "Minute = %M\n";

        if (pIsElementPresent[GenericTools::Tuple::IndexOf<Private::Second, DateTuple>::value])
			format << "Second = %S\n";

		return Yuni::DateTime::TimestampToString(out, format, timeStamp(), false);
	}


	inline time_t Date::timeStamp() const
	{
		return ::mktime(&pData);
	}


	template<class T>
	void Date::constructorHelper(std::array<int, dateTupleSize>& out, int value)
	{
        enum { temp = GenericTools::Tuple::IndexOf<T, DateTuple>::value };
		YUNI_STATIC_ASSERT(( temp != -1),
			TheTemplateParameterDoesntExistInDateTuple);

		size_t index = static_cast<size_t>(temp);

		assert(pIsElementPresent[temp] == false && "Should be initialized at false");

		if (value != -1)
		{
			pIsElementPresent.set(index);
			out[index] = value;
		}
	}


	template<>
	inline void Date::get<Private::Year>(DateString& out) const
	{
		Yuni::DateTime::TimestampToString(out, "%Y", timeStamp(), false);
	}

	template<>
	inline void Date::get<Private::Month>(DateString& out) const
	{
		Yuni::DateTime::TimestampToString(out, "%m", timeStamp(), false);
	}

	template<>
	inline void Date::get<Private::Day>(DateString& out) const
	{
		Yuni::DateTime::TimestampToString(out, "%d", timeStamp(), false);
	}

	template<>
	inline void Date::get<Private::Hour>(DateString& out) const
	{
		Yuni::DateTime::TimestampToString(out, "%H", timeStamp(), false);
	}

	template<>
	inline void Date::get<Private::Minute>(DateString& out) const
	{
		Yuni::DateTime::TimestampToString(out, "%M", timeStamp(), false);
	}

	template<>
	inline void Date::get<Private::Second>(DateString& out) const
	{
		Yuni::DateTime::TimestampToString(out, "%S", timeStamp(), false);
	}

	template<class  T>
	inline void Date::set(const DateString& in)
	{
        enum { value = GenericTools::Tuple::IndexOf<T, DateTuple>::value };
		if (in.empty())
			pIsElementPresent.reset(value);
		else
		{
			int buf(in.to<int>());
			pIsElementPresent.set(value);
			toCTimeInformations<T>(pData, buf);
		}
	}


	template<>
	inline void Date::copy<Private::Year>(const Date& rhs)
	{
		pData.tm_year = rhs.pData.tm_year;
		copyBitsetHelper<Private::Year>(rhs);
	}

	template<>
	inline void Date::copy<Private::Month>(const Date& rhs)
	{
		pData.tm_mon = rhs.pData.tm_mon;
		copyBitsetHelper<Private::Month>(rhs);
	}

	template<>
	inline void Date::copy<Private::Day>(const Date& rhs)
	{
		pData.tm_mday = rhs.pData.tm_mday;
		copyBitsetHelper<Private::Day>(rhs);
	}


	template<>
	inline void Date::copy<Private::Hour>(const Date& rhs)
	{
		pData.tm_hour = rhs.pData.tm_hour;
		copyBitsetHelper<Private::Hour>(rhs);
	}

	template<>
	inline void Date::copy<Private::Minute>(const Date& rhs)
	{
		pData.tm_min = rhs.pData.tm_min;
		copyBitsetHelper<Private::Minute>(rhs);
	}

	template<>
	inline void Date::copy<Private::Second>(const Date& rhs)
	{
		pData.tm_sec = rhs.pData.tm_sec;
		copyBitsetHelper<Private::Second>(rhs);
	}

	inline bool Date::isEmpty() const
	{
		return pIsElementPresent.none();
	}


	template<class T>
	void Date::copyBitsetHelper(const Date& rhs)
	{
        enum { value = GenericTools::Tuple::IndexOf<T, DateTuple>::value};
		pIsElementPresent[value] = rhs.pIsElementPresent[value];
	}

} // namespace ExtendedPhoto
} // namespace PictStock


#endif /* DATE_HXX_ */
