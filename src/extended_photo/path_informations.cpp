#include "path_informations.hpp"

using namespace Yuni;

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

	template<>
	void PathInformations::setElement<Private::Year>(const AnyString& input)
	{
		pDate.year = input;
	}

	template<>
	void PathInformations::setElement<Private::Month>(const AnyString& input)
	{
		pDate.month = input;
	}

	template<>
	void PathInformations::setElement<Private::Day>(const AnyString& input)
	{
		pDate.day = input;
	}

	template<>
	void PathInformations::setElement<Private::Hour>(const AnyString& input)
	{
		pDate.hour = input;
	}

	template<>
	void PathInformations::setElement<Private::Minute>(const AnyString& input)
	{
		pDate.minute = input;
	}

	template<>
	void PathInformations::setElement<Private::Second>(const AnyString& input)
	{
		pDate.second = input;
	}

	template<>
	void PathInformations::setElement<Private::Photographer>(const AnyString& input)
	{
		std::multimap<std::string, Yuni::String> empty;
		pPhotographerPtr = new Photographer("", input, empty);
	}

	template<>
	void PathInformations::setElement<Private::Year>(const PathInformations& input)
	{
		pDate.year = input.pDate.year;
	}

	template<>
	void PathInformations::setElement<Private::Month>(const PathInformations& input)
	{
		pDate.month = input.pDate.month;
	}

	template<>
	void PathInformations::setElement<Private::Day>(const PathInformations& input)
	{
		pDate.day = input.pDate.day;
	}

	template<>
	void PathInformations::setElement<Private::Hour>(const PathInformations& input)
	{
		pDate.hour = input.pDate.hour;
	}

	template<>
	void PathInformations::setElement<Private::Minute>(const PathInformations& input)
	{
		pDate.minute = input.pDate.minute;
	}

	template<>
	void PathInformations::setElement<Private::Second>(const PathInformations& input)
	{
		pDate.second = input.pDate.second;
	}

	template<>
	void PathInformations::setElement<Private::Photographer>(const PathInformations& input)
	{
		pPhotographerPtr = input.pPhotographerPtr; // valid even with nullptr
	}


	template<>
	AnyString PathInformations::getElement<Private::Year>() const
	{
		return pDate.year;
	}

	template<>
	AnyString PathInformations::getElement<Private::Month>() const
	{
		return pDate.month;
	}

	template<>
	AnyString PathInformations::getElement<Private::Day>() const
	{
		return pDate.day;
	}

	template<>
	AnyString PathInformations::getElement<Private::Hour>() const
	{
		return pDate.hour;
	}

	template<>
	AnyString PathInformations::getElement<Private::Minute>() const
	{
		return pDate.minute;
	}

	template<>
	AnyString PathInformations::getElement<Private::Second>() const
	{
		return pDate.second;
	}

	template<>
	AnyString PathInformations::getElement<Private::Photographer>() const
	{
		assert(!(!pPhotographerPtr));
		return pPhotographerPtr->abbr();
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
