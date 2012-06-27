#include <algorithm>
#include "../pict_stock.hpp"
#include "date.hpp"
#include "private/date_helpers.hpp"

# ifdef USE_BOOST_REGULAR_EXPR
#  include <boost/regex.hpp>
namespace regexNS = boost;
# else // USE_BOOST_REGULAR_EXPR
#  include <regex>
namespace regexNS = std;
# endif // USE_BOOST_REGULAR_EXPR



namespace PictStock
{

	namespace
	{
		static const YString expression = 
			YString('^')
			<< '(' << Private::Year::Regex() << "|    )"
			<< ':' // separator
			<< '(' << Private::Month::Regex() << "|  )"
			<< ':' // separator
			<< '(' << Private::Day::Regex() << "|  )"
			<< ' ' // separator
			<< '(' << Private::Hour::Regex() << "|  )"
			<< ':' // separator
			<< '(' << Private::Minute::Regex() << "|  )"
			<< ':' // separator
			<< '(' << Private::Second::Regex() << "|)"
			<< '$';

		/*!
		** \brief Regular expression for date formatting
		**
		** Basically format is YYYY:MM:DD HH:mm:SS
		**
		** IMPORTANT: If this is modified check #Date::dateFromExif method remains correct!
		*/
		static const regexNS::regex RegexDateFormatting(expression.c_str());


		/*!
		** \brief Default value for tm object
		**
		** Original default value get all values at INT_MAX; I'd rather put them
		** at 0 (so for instance time is unknown it is put at 00:00:00 instead
		** of INT_MAX which would also change entirely the date - I'm not very
		** fond of default choices in C tm object...
		**
		** So here we put the date that matches time stamp 0
		 */
		static tm DefaultTimeInformations()
		{
			tm ret;

			const time_t defaultTimeStamp = 1; // 1 because 0 is replaced by current time stamp!

			YString out;
			Yuni::DateTime::TimestampToString(out, "%y", defaultTimeStamp, true);
			ret.tm_year = out.to<int>();
			Yuni::DateTime::TimestampToString(out, "%m", defaultTimeStamp, true);
			ret.tm_mon = out.to<int>() - 1;
			Yuni::DateTime::TimestampToString(out, "%d", defaultTimeStamp, true);
			ret.tm_mday = out.to<int>();
			Yuni::DateTime::TimestampToString(out, "%H", defaultTimeStamp, true);
			ret.tm_hour = out.to<int>();
			Yuni::DateTime::TimestampToString(out, "%M", defaultTimeStamp, true);
			ret.tm_min = out.to<int>();
			Yuni::DateTime::TimestampToString(out, "%S", defaultTimeStamp, true);
			ret.tm_sec = out.to<int>() - 1; // -1 because defaultTimeStamp is 1 and we
											// want 0 as a final value
			ret.tm_isdst = 0;

			assert(mktime(&ret) == 0);

			return ret;
		}


	} // anonymous namespace


	Date::Date()
		: pData(DefaultTimeInformations())
	{
		assert("By default bitset should be init at 0 everywhere"
			&& pIsElementPresent.none());
		//pIsElementPresent.reset();
	}


	Date::Date(int year, int month, int day, int hour, int minute, int second)
	{
		std::array<int, dateTupleSize> elements;
		constructorHelper<Private::Year>(elements, year);
		constructorHelper<Private::Month>(elements, month);
		constructorHelper<Private::Day>(elements, day);
		constructorHelper<Private::Hour>(elements, hour);
		constructorHelper<Private::Minute>(elements, minute);
		constructorHelper<Private::Second>(elements, second);

		// Recursively put the data into pData structure
		conversionHelper<0>(elements);
	}

	Date::Date(const std::array<int, 6>& date)
	{
		// TODO: use C++11 call constructor when it is better supported (neither g++4-5
		// nor VisualStudio 10 admit it currently) instead of this quite ugly trick
		*this = Date(date[0], date[1], date[2], date[3], date[4], date[5]);
	}


	Date::Date(const Date& rhs)
		: pData(rhs.pData),
		  pIsElementPresent(rhs.pIsElementPresent)
	{ }


	Date::Date(time_t timeStamp)
	{
		YString out;
		Yuni::DateTime::TimestampToString(out, "%y", timeStamp, true);
		pData.tm_year = out.to<int>();
		Yuni::DateTime::TimestampToString(out, "%m", timeStamp, true);
		pData.tm_mon = out.to<int>() - 1;
		Yuni::DateTime::TimestampToString(out, "%d", timeStamp, true);
		pData.tm_mday = out.to<int>();
		Yuni::DateTime::TimestampToString(out, "%H", timeStamp, true);
		pData.tm_hour = out.to<int>();
		Yuni::DateTime::TimestampToString(out, "%M", timeStamp, true);
		pData.tm_min = out.to<int>();
		Yuni::DateTime::TimestampToString(out, "%S", timeStamp, true);
		pData.tm_sec = out.to<int>();
		pData.tm_isdst = 0;

		pIsElementPresent.set();

		assert(this->timeStamp() == timeStamp);
	}



	void swap(Date& lhs, Date& rhs)
	{
		using std::swap;
		swap(lhs.pData, rhs.pData);
		swap(lhs.pIsElementPresent, rhs.pIsElementPresent);
	}


	Date& Date::operator = (Date rhs)
	{
		// Nifty way to define operator= in term of recopy constructor
		// (see Herb Sutter's Exceptional C++, Item 12)
		swap(rhs, *this);
		return *this;
	}



	bool dateFromExif(LoggingFacility& logs, Date& out, const YString& dateRead)
	{
		regexNS::cmatch match;

		if (regex_search(dateRead.c_str(), match, RegexDateFormatting))
		{
			enum { dateTupleSize =  std::tuple_size<DateTuple>::value };

			assert(match.size() == dateTupleSize + 1u
				&& "First one is complete expression, others the sub-expressions");

			std::array<int, dateTupleSize> elements;

			// We assume here the order of RegexDateFormatting is year, month, day, hour,
			// minute, second (this is true at the time those lines are written and there is
			// no reason at all to change that!)
			for (int i = 0; i < dateTupleSize; ++i)
			{
				Date::DateString buf = match[i + 1].str();
				buf.trim();
				if (buf.notEmpty())
					elements[static_cast<size_t>(i)] = buf.to<int>();
				else
					elements[static_cast<size_t>(i)] = -1; // default value
			}

			out = Date(elements);

			return true;
		}
		else
		{
			logs.error("Unable to interpret date \"") << dateRead << "\"; does not match expected format";
			return false;
		}
	}


	Yuni::CString<20, false> dateToExif(const Date& date)
	{
		Yuni::CString<20, false> ret;
		Yuni::DateTime::TimestampToString(ret, "%Y:%m:%d   :  :  ", date.timeStamp(), false);
		return ret;
	}


	bool operator == (const Date& lhs, const Date& rhs)
	{
		// We can't compare directly pData as it might differ whether fields
		// yday and wday have yet been filled or not
		return (lhs.timeStamp() == rhs.timeStamp() &&
			lhs.pIsElementPresent == rhs.pIsElementPresent);
	}


	bool operator != (const Date& lhs, const Date& rhs)
	{
		return !(operator == (lhs, rhs));
	}


	bool operator < (const Date& lhs, const Date& rhs)
	{
		// Easy case: both time stamps are different
		auto lTimeStamp = lhs.timeStamp();
		auto rTimeStamp = rhs.timeStamp();

		if (lTimeStamp != rTimeStamp)
			return lTimeStamp < rTimeStamp;

		// If they are equal, we must check the same elements are present
		// For instance, take the two following dates:
		// 2012/06/20 00:00:00 and 2012/06/20 with time unknown
		// We do not want them equal!
		if (lhs.pIsElementPresent == rhs.pIsElementPresent)
			return false;

		// If not equal, we should choose an arbitrary criterion to make sure
		// a < b and b < a can't be both obtained...
		return lhs.pIsElementPresent.to_ulong() < rhs.pIsElementPresent.to_ulong();
	}


	bool operator <= (const Date& lhs, const Date& rhs)
	{
		if (lhs == rhs)
			return true;

		return lhs < rhs;
	}


	bool operator > (const Date& lhs, const Date& rhs)
	{
		return !(operator <= (lhs, rhs));
	}


	bool operator >= (const Date& lhs, const Date& rhs)
	{
		return !(operator < (lhs, rhs));
	}


	template<>
	void Date::toCTimeInformations<Private::Year>(int value)
	{
		pData.tm_year = value - 1900;
	}


	template<>
	void Date::toCTimeInformations<Private::Month>(int value)
	{
		assert(value > 0 && value <= 12);
		pData.tm_mon = --value;
	}

	template<>
	void Date::toCTimeInformations<Private::Day>(int value)
	{
		assert(value >= 1 && value <= 31);
		pData.tm_mday = value;
	}

	template<>
	void Date::toCTimeInformations<Private::Hour>(int value)
	{
		assert(value >= 0 && value <= 23);
		pData.tm_hour = value;
	}

	template<>
	void Date::toCTimeInformations<Private::Minute>(int value)
	{
		assert(value >= 0 && value <= 59);
		pData.tm_min = value;

	}

	template<>
	void Date::toCTimeInformations<Private::Second>(int value)
	{
		assert(value >= 0 && value < 60);

		# ifdef YUNI_OS_WINDOWS
		long timezone;
		_get_timezone(&timezone);
		# endif

		pData.tm_sec = value + static_cast<int>(timezone);
	}



} // namespace PictStock





