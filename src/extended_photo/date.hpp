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
	class YUNI_DECL Date
	{
	public:

		//! Default constructor
		explicit Date();

		//! Recopy constructor
		Date(const Date&);

		//! Assignation
		Date& operator = (Date);

		// 5 = 4 (maximal length for year) + terminal character length
		// Related to a Yuni bug; should be rectified soon!
		typedef Yuni::CString<5, true> DateString;

		//! Smart pointer most adapted for the class
		typedef Yuni::SmartPtr<Date> Ptr;

		//! True if no value is set, false otherwise
		bool isEmpty() const;

		//! Print, for debug purposes
		void print(std::ostream& out) const;

		inline const DateString& year() const;

		inline const DateString& month() const;

		inline const DateString& day() const;

		inline const DateString& hour() const;

		inline const DateString& minute() const;

		inline const DateString& second() const;

		inline void year(const AnyString&);

		inline void month(const AnyString&);

		inline void day(const AnyString&);

		inline void hour(const AnyString&);

		inline void minute(const AnyString&);

		inline void second(const AnyString&);

	private:

		DateString pYear;
		DateString pMonth;
		DateString pDay;
		DateString pHour;
		DateString pMinute;
		DateString pSecond;

		// friendship!
		friend bool operator == (const Date& lhs, const Date& rhs);
		friend bool operator < (const Date& lhs, const Date& rhs);
		friend void swap(Date& lhs, Date& rhs);

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

# include "date.hxx"



#endif /* DATE_HPP_ */
