#include "relevant_informations.hpp"


namespace PictStock
{
namespace Private
{

	RelevantInformations::RelevantInformations(LoggingFacility& logs)
		: logs(logs),
		  pPhotographerPtr(nullptr)
	{ }


	void RelevantInformations::setPhotographer(const Photographer::Ptr& photographer)
	{
		pPhotographerPtr = photographer;
	}


	void RelevantInformations::setDate(const Date& date)
	{
		pDate = date;
	}


	void RelevantInformations::setValue(unsigned int natureIndex, const AnyString& value)
	{
		Date& date = pDate;

		switch(natureIndex)
		{
		case Elements::year:
			date.year = value;
			break;
		case Elements::month:
			date.month = value;
			break;
		case Elements::day:
			date.day = value;
			break;
		case Elements::hour:
			date.hour = value;
			break;
		case Elements::minute:
			date.minute = value;
			break;
		case Elements::second:
			date.second = value;
			break;
		case Elements::photographer:
			{
				std::multimap<std::string, Yuni::String> buf;
				pPhotographerPtr = new Photographer("", value, buf);
				break;
			}
		case Elements::size:
			assert(false && "Should never be invoked");
			logs.fatal("RelevantInformations::value() should never be invoked with very last index");
			exit(EXIT_FAILURE);
			break;
		}// switch

	}


	Yuni::CString<10, false> RelevantInformations::value(unsigned int natureIndex) const
	{
		const Date& date = pDate;

		switch(natureIndex)
		{
		case Elements::year:
			return date.year;
		case Elements::month:
			return date.month;
		case Elements::day:
			return date.day;
		case Elements::hour:
			return date.hour;
		case Elements::minute:
			return date.minute;
		case Elements::second:
			return date.second;
		case Elements::photographer:
			{
				assert(!(!pPhotographerPtr));
				return pPhotographerPtr->abbr();
			}
		case Elements::size:
			assert(false && "Should never be invoked");
			logs.fatal("RelevantInformations::value() should never be invoked with very last index");
			exit(EXIT_FAILURE);
			break;
		}// switch

		return ""; // to avoid compilation warning
	}


	void RelevantInformations::changeDate(const Yuni::CString<8, false>& newDate)
	{
		auto& date = pDate;

		date.year.assign(newDate, 4);
		date.month.assign(newDate, 2, 4);
		date.day.assign(newDate, 2, 6);

		date.hour = "  ";
		date.minute = "  ";
		date.second = "  ";
	}


	RelevantInformations RelevantInformations::onlyUsefulOnes(
		const std::bitset<Elements::size>& arePresent) const
	{
		RelevantInformations ret(logs);

		for (unsigned int i = 0u; i < Elements::size; ++i)
		{
			if (arePresent.test(i))
				ret.setValue(i, value(i));
		}

		return ret;
	}


	bool RelevantInformations::isEmpty() const
	{
		if (!pPhotographerPtr)
			return false;

		return pDate.isEmpty();
	}


	bool operator==(const RelevantInformations& lhs, const RelevantInformations& rhs)
	{
		if (lhs.pDate != rhs.pDate)
			return false;

		if (lhs.pPhotographerPtr == rhs.pPhotographerPtr)
			return true;

		assert(*(lhs.pPhotographerPtr) != *(rhs.pPhotographerPtr) &&
			"If not, that means same photographer was dynamically created "
			"twice somewhere, which should never happen");

		return false;
	}


	bool operator!=(const RelevantInformations& lhs, const RelevantInformations& rhs)
	{
		return !(operator == (lhs, rhs));
	}

	bool operator<(const RelevantInformations& lhs, const RelevantInformations& rhs)
	{
		if (lhs.pDate == rhs.pDate)
			return false;

		if (rhs.pDate < lhs.pDate)
			return false;

		return (lhs.pPhotographerPtr < rhs.pPhotographerPtr);
	}

} // namespace Private
} // namespace PictStock
