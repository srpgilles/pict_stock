#ifndef DATE_HXX_
# define DATE_HXX_


namespace PictStock
{

	template<>
	inline void Date::toCTimeInformations<Private::Year>(int value)
	{
		pData.tm_year = value - 1900;
	}


	template<>
	inline void Date::toCTimeInformations<Private::Month>(int value)
	{
		assert(value > 0 && value <= 12);
		pData.tm_mon = --value;
	}

	template<>
	inline void Date::toCTimeInformations<Private::Day>(int value)
	{
		assert(value >= 1 && value <= 31);
		pData.tm_mday = value;
	}

	template<>
	inline void Date::toCTimeInformations<Private::Hour>(int value)
	{
		assert(value >= 0 && value <= 23);
		pData.tm_hour = value;
	}

	template<>
	inline void Date::toCTimeInformations<Private::Minute>(int value)
	{
		assert(value >= 0 && value <= 59);
		pData.tm_min = value;

	}

	template<>
	inline void Date::toCTimeInformations<Private::Second>(int value)
	{
		assert(value >= 0 && value < 60);
		pData.tm_sec = value + static_cast<int>(timezone);
	}


	/*!
	** \brief Helper to perform recursively all required conversions
	**
	** IMPORTANT: it is assumed pIsElementPresent has been filled along
	** with input array
	**
	** \param[in] in Array that contains original values that must be converted
	** to fit inside tm object. For instance, in[Element::year] = 2012 yields
	** to 112 in pData structure
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
			toCTimeInformations<DateTuple[I]>(in[I]);

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


		YUNI_STATIC_ASSERT((GenericTools::IndexOf<Private::Year, DateTuple>::value >= 0),
			YearNotFoundInTuple);
		YUNI_STATIC_ASSERT((GenericTools::IndexOf<Private::Month, DateTuple>::value >= 0),
			MonthNotFoundInTuple);
		YUNI_STATIC_ASSERT((GenericTools::IndexOf<Private::Day, DateTuple>::value >= 0),
			DayNotFoundInTuple);
		YUNI_STATIC_ASSERT((GenericTools::IndexOf<Private::Hour, DateTuple>::value >= 0),
			HourNotFoundInTuple);
		YUNI_STATIC_ASSERT((GenericTools::IndexOf<Private::Minute, DateTuple>::value >= 0),
			MinuteNotFoundInTuple);
		YUNI_STATIC_ASSERT((GenericTools::IndexOf<Private::Second, DateTuple>::value >= 0),
			SecondNotFoundInTuple);


		if (pIsElementPresent[GenericTools::IndexOf<Private::Year, DateTuple>::value])
			format << "Year = %Y\n";

		if (pIsElementPresent[GenericTools::IndexOf<Private::Month, DateTuple>::value])
			format << "Month = %m\n";

		if (pIsElementPresent[GenericTools::IndexOf<Private::Day, DateTuple>::value])
			format << "Day = %d\n";

		if (pIsElementPresent[GenericTools::IndexOf<Private::Hour, DateTuple>::value])
			format << "Hour = %H\n";

		if (pIsElementPresent[GenericTools::IndexOf<Private::Minute, DateTuple>::value])
			format << "Minute = %M\n";

		if (pIsElementPresent[GenericTools::IndexOf<Private::Second, DateTuple>::value])
			format << "Second = %S\n";

		return Yuni::DateTime::TimestampToString(out, format, timeStamp(), false);
	}


	inline time_t Date::timeStamp() const
	{
		return ::mktime(&pData);
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
		int buf(in.to<int>());
		toCTimeInformations<T>(buf);
	}


	template<>
	inline void Date::copy<Private::Year>(const Date& rhs) const
	{
		pData.tm_year = rhs.pData.tm_year;
	}

	template<>
	inline void Date::copy<Private::Month>(const Date& rhs) const
	{
		pData.tm_mon = rhs.pData.tm_mon;
	}

	template<>
	inline void Date::copy<Private::Day>(const Date& rhs) const
	{
		pData.tm_mday = rhs.pData.tm_mday;
	}


	template<>
	inline void Date::copy<Private::Hour>(const Date& rhs) const
	{
		pData.tm_hour = rhs.pData.tm_hour;
	}

	template<>
	inline void Date::copy<Private::Minute>(const Date& rhs) const
	{
		pData.tm_min = rhs.pData.tm_min;
	}

	template<>
	inline void Date::copy<Private::Second>(const Date& rhs) const
	{
		pData.tm_sec = rhs.pData.tm_sec;
	}

	inline bool Date::isEmpty() const
	{
		return pIsElementPresent.none();
	}


} // namespace PictStock




#endif /* DATE_HXX_ */
