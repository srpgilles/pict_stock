#ifndef DATE_HPP_
# define DATE_HPP_

#include <yuni/core/string.h>
#include "../pict_stock.hpp"

namespace PictStock
{
namespace Private
{


	/*!
	** \brief A very simple class to contain date informations
	**
	** Is intended to be initialized only by dateFromExif function at the moment
	*/
	struct YUNI_DECL Date
	{
		typedef Yuni::CString<4, false> DateString;

		DateString year;
		DateString month;
		DateString day;
		DateString hour;
		DateString minute;
		DateString second;
	};

	//! Interpret the string dateRead and extract from it the date.
	bool dateFromExif(LoggingFacility& logs, Date& out, const YString& dateRead);

} // namespace Private
} // namespace PictStock




#endif /* DATE_HPP_ */