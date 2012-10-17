#include "path_informations.hpp"

using namespace Yuni;

namespace PictStock
{
namespace ExtendedPhoto
{

	PathInformations::PathInformations(LoggingFacility& logs)
		: logs(logs),
		  pPhotographerPtr(nullptr)
	{ }


	PathInformations::PathInformations(const PathInformations& rhs)
		: logs(rhs.logs),
		  pPhotographerPtr(rhs.pPhotographerPtr),
		  pDate(rhs.pDate)
	{ }


	PathInformations& PathInformations::operator = (const PathInformations& rhs)
	{
		pPhotographerPtr = rhs.pPhotographerPtr;
		pDate = rhs.pDate;

		return *this;
	}


	void PathInformations::changeDate(const Yuni::CString<8, false>& newDate)
	{
		auto& date = pDate;

		Date::DateString buf;
		buf.assign(newDate, 4);
		date.set<Private::Year>(buf);
		buf.clear();
		buf.assign(newDate, 2, 4);
		date.set<Private::Month>(buf);
		buf.clear();
		buf.assign(newDate, 2, 6);
		date.set<Private::Day>(buf);

		date.set<Private::Hour>("");
		date.set<Private::Minute>("");
		date.set<Private::Second>("");
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
		{
			String buf;
			pDate.print(buf);
			out << buf;
		}
		out << "\tPhotographer:\n";
		if (!pPhotographerPtr)
			out << "Empty!" << '\n';
		else
			out << pPhotographerPtr->abbr();
	}

} // namespace ExtendedPhoto
} // namespace PictStock
