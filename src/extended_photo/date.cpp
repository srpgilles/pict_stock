#include <algorithm>
#include "../pict_stock.hpp"
#include "date.hpp"
#include "private/date_helpers.hpp"

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

			ret.tm_year = 70;
			ret.tm_mon = 0;
			ret.tm_mday = 1;
			ret.tm_hour = 0;
			ret.tm_min = 0;
			ret.tm_sec = - static_cast<int>(timezone);
			ret.tm_isdst = 0;

			assert(mktime(&ret) == 0);

			return ret;
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
			conversionHelper(
				tm& /*out*/,
				const std::array<int, std::tuple_size<DateTuple>::value>& /*in*/,
				const std::bitset<std::tuple_size<DateTuple>::value>& /*isElementPresent*/
				)
		{ }

		template<std::size_t I>
		typename std::enable_if<I < std::tuple_size<DateTuple>::value, void>::type
			conversionHelper(
				tm& out,
				const std::array<int, std::tuple_size<DateTuple>::value>& in,
				const std::bitset<std::tuple_size<DateTuple>::value>& isElementPresent
				)
		{
			if (isElementPresent[I])
				toCTimeInformations<DateTuple[I]>(out, in[I]);

			conversionHelper<I+1>(out, in, isElementPresent);
		}





	} // anonymous namespace


	Date::Date()
		: pData(DefaultTimeInformations())
	{
		assert("By default bitset should be init at 0 everywhere"
			&& pIsElementPresent.none());
		//pIsElementPresent.reset();
	}


	Date::Date(const regexNS::cmatch& regexMatch)
		: Date()
	{
		enum { size = std::tuple_size<DateTuple>::value };

		assert("First one is complete expression, others the sub-expressions"
			&& regexMatch.size() == size);

		std::array<int, size> elements;

		for (int i = 0; i < size; ++i)
		{
			DateString buf = regexMatch[i + 1].str();
			buf.trim();
			if (buf.notEmpty())
			{
				pIsElementPresent.set(static_cast<size_t>(i));
				elements[static_cast<size_t>(i)] = buf.to<int>();
			}
		}

		// Recursively put the data into pData structure
		conversionHelper<0>(pData, elements, pIsElementPresent);
	}


	Date::Date(const Date& rhs)
		: pData(rhs.pData),
		  pIsElementPresent(rhs.pIsElementPresent)
	{ }


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
			assert(match.size() == 7u && "First one is complete expression, others the sub-expressions");

			Date buf(match);
			out = buf;

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






} // namespace PictStock





