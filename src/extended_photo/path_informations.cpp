#include "path_informations.hpp"


namespace PictStock
{

	PathInformations::PathInformations(LoggingFacility& logs)
		: logs(logs),
		  pPhotographerPtr(nullptr)
	{ }


	void PathInformations::setPhotographer(const Photographer::Ptr& photographer)
	{
		pPhotographerPtr = photographer;
	}


	void PathInformations::setDate(const Date& date)
	{
		pDate = date;
	}


	void PathInformations::setValue(unsigned int natureIndex, const AnyString& value)
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


	Yuni::CString<10, false> PathInformations::value(unsigned int natureIndex) const
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
				if (!pPhotographerPtr)
					return "UNK";

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


	void PathInformations::changeDate(const Yuni::CString<8, false>& newDate)
	{
		auto& date = pDate;

		date.year.assign(newDate, 4);
		date.month.assign(newDate, 2, 4);
		date.day.assign(newDate, 2, 6);

		date.hour = "  ";
		date.minute = "  ";
		date.second = "  ";
	}


	PathInformations PathInformations::onlyUsefulOnes(
		const std::bitset<Elements::size>& arePresent) const
	{
		PathInformations ret(logs);

		for (unsigned int i = 0u; i < Elements::size; ++i)
		{
			if (arePresent.test(i))
				ret.setValue(i, value(i));
		}

		return ret;
	}


	bool PathInformations::isEmpty() const
	{
		if (!(!pPhotographerPtr))
			return false;

		return pDate.isEmpty();
	}


	bool operator==(const PathInformations& lhs, const PathInformations& rhs)
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


	bool operator!=(const PathInformations& lhs, const PathInformations& rhs)
	{
		return !(operator == (lhs, rhs));
	}


	bool operator<(const PathInformations& lhs, const PathInformations& rhs)
	{
		if (lhs.pDate == rhs.pDate)
			return false;

		if (rhs.pDate < lhs.pDate)
			return false;

		// Totally subjective here; the point is to ensure key in map container
		if (!(lhs.pPhotographerPtr))
			return true;

		if (!(rhs.pPhotographerPtr))
			return false; // opposite of the one above to ensure consistency

		return (lhs.pPhotographerPtr < rhs.pPhotographerPtr);
	}


	void PathInformations::print(std::ostream& out) const
	{
		out << "Relevant informations are:\n\tDate:\n";
		pDate.print(out);
		out << "\tPhotographer:\n";
		if (!pPhotographerPtr)
			out << "Empty!" << '\n';
		else
			out << pPhotographerPtr->abbr();
	}

} // namespace PictStock