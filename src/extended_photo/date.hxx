#ifndef DATE_HXX_
# define DATE_HXX_

namespace PictStock
{


	inline const Date::DateString& Date::year() const
	{
		return pYear;
	}


	inline const Date::DateString& Date::month() const
	{
		return pMonth;
	}


	inline const Date::DateString& Date::day() const
	{
		return pDay;
	}


	inline const Date::DateString& Date::hour() const
	{
		return pHour;
	}


	inline const Date::DateString& Date::minute() const
	{
		return pMinute;
	}


	inline const Date::DateString& Date::second() const
	{
		return pSecond;
	}


	inline void Date::year(const AnyString& value)
	{
		pYear = value;
	}


	inline void Date::month(const AnyString& value)
	{
		pMonth = value;
	}


	inline void Date::day(const AnyString& value)
	{
		pDay = value;
	}

	inline void Date::hour(const AnyString& value)
	{
		pHour = value;
	}


	inline void Date::minute(const AnyString& value)
	{
		pMinute = value;
	}


	inline void Date::second(const AnyString& value)
	{
		pSecond = value;
	}



} // namespace PictStock




#endif /* DATE_HXX_ */
