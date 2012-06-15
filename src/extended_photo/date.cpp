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
		: year(rhs.year),
		  month(rhs.month),
		  day(rhs.day),
		  hour(rhs.hour),
		  minute(rhs.minute),
		  second(rhs.second)
	{ }


	Date& Date::operator = (const Date& rhs)
	{
		// Nifty way to define operator= in term of recopy constructor
		// (see Herb Sutter's Exceptional C++, Item 12)
		Date buf(rhs);
		std::swap(buf, *this);
		return *this;
	}



	bool dateFromExif(LoggingFacility& logs, Date& out, const YString& dateRead)
	{
		regexNS::cmatch match;

		if (regex_search(dateRead.c_str(), match, RegexDateFormatting))
		{
			assert(match.size() == 7u && "First one is complete expression, others the sub-expressions");
			out.year = match[1].str();
			out.month = match[2].str();
			out.day = match[3].str();
			out.hour = match[4].str();
			out.minute = match[5].str();
			out.second = match[6].str();

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
		Yuni::CString<20, false> ret(date.year);
		ret << ':' << date.month << ':' << date.day << "   :  :  ";
		return ret;
	}


	bool operator == (const Date& lhs, const Date& rhs)
	{
		return lhs.year == rhs.year &&
			lhs.month == rhs.month &&
			lhs.day == rhs.day &&
			lhs.hour == rhs.hour &&
			lhs.minute == rhs.minute &&
			lhs.second == rhs.second;
	}


	bool operator != (const Date& lhs, const Date& rhs)
	{
		return !(operator == (lhs, rhs));
	}


	bool operator < (const Date& lhs, const Date& rhs)
	{
		if (lhs.year < rhs.year)
			return true;

		if (lhs.month < rhs.month)
			return true;

		if (lhs.day < rhs.day)
			return true;

		if (lhs.hour < rhs.hour)
			return true;

		if (lhs.minute < rhs.minute)
			return true;

		return (lhs.second < rhs.second);
	}


	bool Date::isEmpty() const
	{
		return year.empty()
			&& month.empty()
			&& day.empty()
			&& hour.empty()
			&& minute.empty()
			&& second.empty();
	}


	void Date::print(std::ostream& out) const
	{
		if (isEmpty())
		{
			out << "Empty!" << '\n';
			return;
		}

		if (!year.empty())
			out << "Year = " << year << '\n';

		if (!month.empty())
			out << "Month = " << month << '\n';

		if (!day.empty())
			out << "Day = " << day << '\n';

		if (!hour.empty())
			out << "Hour = " << hour << '\n';

		if (!minute.empty())
			out << "Minute = " << minute << '\n';

		if (!second.empty())
			out << "Second = " << second << '\n';
	}




} // namespace PictStock





