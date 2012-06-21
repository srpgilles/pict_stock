#ifndef DATE_HXX_
# define DATE_HXX_


namespace PictStock
{
	namespace // anonymous
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
		** \param[out] out Struct tm in which the new value is placed
		** \param[in] value Value befoire conversion (for instance 2012 for year)
		*/
		template<class T>
		void toCTimeInformations(tm& out, int value);


		template<>
		inline void toCTimeInformations<Private::Year>(tm& out, int value)
		{
			out.tm_year = value - 1900;
		}


		template<>
		inline void toCTimeInformations<Private::Month>(tm& out, int value)
		{
			assert(value > 0 && value <= 12);
			out.tm_mon = --value;
		}

		template<>
		inline void toCTimeInformations<Private::Day>(tm& out, int value)
		{
			assert(value >= 1 && value <= 31);
			out.tm_mday = value;
		}

		template<>
		inline void toCTimeInformations<Private::Hour>(tm& out, int value)
		{
			assert(value >= 0 && value <= 23);
			out.tm_hour = value;
		}

		template<>
		inline void toCTimeInformations<Private::Minute>(tm& out, int value)
		{
			assert(value >= 0 && value <= 59);
			out.tm_min = value;

		}

		template<>
		inline void toCTimeInformations<Private::Second>(tm& out, int value)
		{
			assert(value >= 0 && value < 60);
			out.tm_sec = value + static_cast<int>(timezone);
		}
	} // namespace anonymous


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
		toCTimeInformations<T>(pData, buf);
	}

	inline bool Date::isEmpty() const
	{
		return pIsElementPresent.none();
	}


} // namespace PictStock




#endif /* DATE_HXX_ */
