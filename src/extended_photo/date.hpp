#ifndef DATE_HPP_
# define DATE_HPP_

# include <yuni/core/string.h>
# include <ostream>
# include "../pict_stock.hpp"

namespace PictStock
{


	/*!
	** \brief A very simple class to contain date informations
	**
	** Is intended to be initialized only by dateFromExif function at the moment
	*/
	struct YUNI_DECL Date
	{
		//! Default constructor
		explicit Date();

		//! Recopy constructor
		Date(const Date&);

		//! Assignation
		Date& operator = (Date);


		typedef Yuni::CString<5, true> DateString; // 5 = 4 (maximal length for year) + terminal character length

		//! Smart pointer most adapted for the class
		typedef Yuni::SmartPtr<Date> Ptr;

		DateString year;
		DateString month;
		DateString day;
		DateString hour;
		DateString minute;
		DateString second;

		//! True if no value is set, false otherwise
		bool isEmpty() const;

		//! Print, for debug purposes
		void print(std::ostream& out) const;



	};

	bool operator == (const Date& lhs, const Date& rhs);
	bool operator != (const Date& lhs, const Date& rhs);
	bool operator < (const Date& lhs, const Date& rhs);
	void swap(Date& lhs, Date& rhs);

	//! Interpret the string dateRead and extract from it the date.
	bool dateFromExif(LoggingFacility& logs, Date& out, const YString& dateRead);

	//! Read the date and create the date as it would be written in exif file
	Yuni::CString<20, false> dateToExif(const Date& date);

} // namespace PictStock




#endif /* DATE_HPP_ */
