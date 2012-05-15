#ifndef DATE_HPP_
# define DATE_HPP_

#include <yuni/core/string.h>

namespace PictStock
{
namespace Private
{
	static const YString yearRegex = "( \\d{1}|0\\d{1}|10|11|12)";
	static const YString monthRegex = "( \\d{1}|0\\d{1}|10|11|12)";
	static const YString dayRegex = "([ |0|1|2]\\d{1}|3[0|1])"; // no check for adequacy month day (for instance 30 Feb is allowed)
	static const YString hourRegex = "([ |0|1]\\d{1}|2[0-4]|  )"; // empty allowed
	static const YString minuteRegex = "([ 0-5]\\d{1}|  )"; // empty is allowed
	static const YString secondRegex(minuteRegex); // empty is allowed

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
