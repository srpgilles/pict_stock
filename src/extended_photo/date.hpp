#ifndef DATE_HPP_
# define DATE_HPP_

# include <yuni/core/string.h>
# include <yuni/datetime/timestamp.h>
# include <ostream>
# include <bitset>
# include "../pict_stock.hpp"
# include "../tools.hpp"
# include <ctime>
# include <array>
# include "private/date_helpers.hpp"

# ifdef USE_BOOST_REGULAR_EXPR
#  include <boost/regex.hpp>
namespace regexNS = boost;
# else // USE_BOOST_REGULAR_EXPR
#  include <regex>
namespace regexNS = std;
# endif // USE_BOOST_REGULAR_EXPR


namespace PictStock
{

//	// TMP TODO Use a tuple instead (and date_helpers defined structures)
//	namespace Element // use enum class when MSVC will support it!
//	{
//		enum Date
//		{
//			year = 0,
//			month,
//			day,
//			hour,
//			minute,
//			second,
//			size
//		};
//	}

	typedef std::tuple<Private::Year, Private::Month, Private::Day,
		Private::Hour, Private::Minute, Private::Second> DateTuple;


	/*!
	** \brief A very simple class to contain date informations
	**
	** It is essentially a thin wrapper over struct tm (from ctime); but
	** the interface is much more user-friendly. For instance (so we get rid of
	** idiosyncrasies):
	** - Year is plain year (not year - 1900)
	** - Month is in [1-12] (instead if 0-11)
	** - During tm -> time stamp conversion, there is an issue because mktime
	** assumed tm object is in local time. I do not consider at all time zone
	** in PictStock (date is read as it is written in the camera), so Date is
	** done as if all was in UTC
	**
	** It is not intended to be as generic as struct tm: the point is mainly to
	** provide what is needed for PictStock project
	*/
	class YUNI_DECL Date
	{
	public:

		//! Constructors
		//@{
		//! Default constructor
		explicit Date();

		/*!
		** \brief Constructor from the result of a regex
		**
		** The regex is defined in anonymous namespace of file date.cpp
		** under the name RegexDateFormatting
		*/
		Date(const regexNS::cmatch& regexMatch);

		//! Recopy constructor
		Date(const Date&);
		//@}

		// 5 = 4 (maximal length for year) + terminal character length
		// Related to a Yuni bug; should be rectified soon!
		typedef Yuni::CString<5, false> DateString;

		//! Assignation
		Date& operator = (Date);

		//! Get the value of one of the date element (as a string)
		template<class T>
		inline void get(DateString& out) const;

		//! Get the value of one of the date element (as a string)
		template<class T>
		inline void set(const DateString& in);

		//! Return time stamp associated to the date
		time_t timeStamp() const;



		//! Print, for debug purposes
		template <class StringT>
		bool print(StringT& out) const;

		//! True if is empty
		inline bool isEmpty() const;

	private:


		/*!
		** \brief Unix time stamp reflecting the date
		**
		** As mentioned in #pIsElementPresent, it might be an accurate time or
		** a rounding if the original date was not precise enough
		**
		** This member is mutable due to its C characteristics: two days are
		** modified when mktime is called to determine time stamp...
		**
		** However do not modify it directly; only one which can do that are C
		** functions!
		 */
		mutable struct tm pData;


		/*!
		** \brief Tells which elements were truly given in input
		**
		** A time stamp is generated for all dates, even if they were actually
		** incomplete to begin with (for instance year, month and day present
		** but hour unknown)
		**
		** Following bitset tells for each #DateTuple whether they were actually
		** there or if a default value was used instead (default values are the
		** first possible entry: 0 for hour, 1 for day, etc...)
		*/
		std::bitset<std::tuple_size<DateTuple>::value> pIsElementPresent;


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
