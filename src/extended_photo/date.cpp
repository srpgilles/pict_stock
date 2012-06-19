#include <algorithm>
#include "../pict_stock.hpp"
#include "date.hpp"
#include "private/date_helpers.hpp"

#ifdef USE_BOOST_REGULAR_EXPR
# include <boost/regex.hpp>
namespace regexNS = boost;
# else // USE_BOOST_REGULAR_EXPR
# include <regex>
namespace regexNS = std;
#endif // USE_BOOST_REGULAR_EXPR




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
		*/
		static const regexNS::regex RegexDateFormatting(expression.c_str());
	} // anonymous namespace


	Date::Date()
	{ }


	Date::Date(const Date& rhs)
		: pYear(rhs.pYear),
		  pMonth(rhs.pMonth),
		  pDay(rhs.pDay),
		  pHour(rhs.pHour),
		  pMinute(rhs.pMinute),
		  pSecond(rhs.pSecond)
	{ }


	void swap(Date& lhs, Date& rhs)
	{
		using std::swap;
		swap(lhs.pYear, rhs.pYear);
		swap(lhs.pMonth, rhs.pMonth);
		swap(lhs.pDay, rhs.pDay);
		swap(lhs.pHour, rhs.pHour);
		swap(lhs.pMinute, rhs.pMinute);
		swap(lhs.pSecond, rhs.pSecond);
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
			assert(match.size() == 7u && "First one is complete expression, others the sub-expressions");
			out.year(match[1].str());
			out.month(match[2].str());
			out.day(match[3].str());
			out.hour(match[4].str());
			out.minute(match[5].str());
			out.second(match[6].str());

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
		Yuni::CString<20, false> ret(date.year());
		ret << ':' << date.month() << ':' << date.day() << "   :  :  ";
		return ret;
	}


	bool operator == (const Date& lhs, const Date& rhs)
	{
		return lhs.pYear == rhs.pYear &&
			lhs.pMonth == rhs.pMonth &&
			lhs.pDay == rhs.pDay &&
			lhs.pHour == rhs.pHour &&
			lhs.pMinute == rhs.pMinute &&
			lhs.pSecond == rhs.pSecond;
	}


	bool operator != (const Date& lhs, const Date& rhs)
	{
		return !(operator == (lhs, rhs));
	}


	bool operator < (const Date& lhs, const Date& rhs)
	{
		// Time stamp much more easier when implemented (but I have to see
		// how to do it properly with MSVC)
		if (lhs.pYear != rhs.pYear)
			return (lhs.pYear < rhs.pYear);

		if (lhs.pMonth != rhs.pMonth)
			return (lhs.pMonth < rhs.pMonth);

		if (lhs.pDay != rhs.pDay)
			return (lhs.pDay < rhs.pDay);

		if (lhs.pHour != rhs.pHour)
			return (lhs.pHour < rhs.pHour);

		if (lhs.pMinute != rhs.pMinute)
			return (lhs.pMinute < rhs.pMinute);

		return (lhs.pSecond < rhs.pSecond);
	}


	bool Date::isEmpty() const
	{
		return pYear.empty()
			&& pMonth.empty()
			&& pDay.empty()
			&& pHour.empty()
			&& pMinute.empty()
			&& pSecond.empty();
	}


	void Date::print(std::ostream& out) const
	{
		if (isEmpty())
		{
			out << "Empty!" << '\n';
			return;
		}

		if (!pYear.empty())
			out << "Year = " << pYear << '\n';

		if (!pMonth.empty())
			out << "Month = " << pMonth << '\n';

		if (!pDay.empty())
			out << "Day = " << pDay << '\n';

		if (!pHour.empty())
			out << "Hour = " << pHour << '\n';

		if (!pMinute.empty())
			out << "Minute = " << pMinute << '\n';

		if (!pSecond.empty())
			out << "Second = " << pSecond << '\n';
	}




} // namespace PictStock





