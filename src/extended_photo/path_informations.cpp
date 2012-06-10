#include "path_informations.hpp"

using namespace Yuni;

namespace PictStock
{

	PathInformations::PathInformations(LoggingFacility& logs)
		: logs(logs),
		  pPhotographerPtr(nullptr)
	{ }


	void PathInformations::changeDate(const Yuni::CString<8, false>& newDate)
	{
		auto& date = pDate;

		date.year.assign(newDate, 4);
		date.month.assign(newDate, 2, 4);
		date.day.assign(newDate, 2, 6);

		date.hour.clear();
		date.minute.clear();
		date.second.clear();
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
