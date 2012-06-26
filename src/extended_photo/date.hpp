#ifndef DATE_HPP_
# define DATE_HPP_

# include <yuni/core/string.h>
# include <yuni/datetime/timestamp.h>
# include <ostream>
# include <bitset>
# include "../pict_stock.hpp"
# include "../tools/tools.hpp"
# include <ctime>
# include <array>
# include "private/date_helpers.hpp"
# include <tuple>


namespace PictStock
{

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
		** \brief Constructor
		**
		** \param Year Put -1 if not known
		** \param Month Put -1 if not known
		** \param Hour Put -1 if not known
		** \param Day Put -1 if not known
		** \param Minute Put -1 if not known
		** \param Second Put -1 if not known
		*/
		Date(int year, int month, int day, int hour = -1, int minute = -1, int second = -1);

		/*!
		** \brief Constructor
		**
		** Same as above except argument is there an array with six arguments
		** (year, month, day, hour, minute, second respectively)
		*/
		explicit Date(const std::array<int, 6>& date);

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

		/*!
		** \brief Copy the value from another #Date object
		**
		** \tparam T An element of DateTuple
		*/
		template<class T>
		inline void copy(const Date&);

		//! Set the value of one of the date element (given as a string in input)
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
		** \brief Function in charge of converting date element into
		** proper struct tm informations
		**
		** For instance, for year 2012 would become 112 (as struct tm
		** elements begin at 1900 for the year)
		**
		** \tparam An element of DateTuple
		**
		** \param[in] value Value before conversion (for instance 2012 for year)
		*/
		template<class T>
		void toCTimeInformations(int value);


		/*!
		** \brief Helper to perform recursively all required conversions
		**
		** IMPORTANT: it is assumed pIsElementPresent has been filled along
		** with input array
		**
		** \param[in] in Array that contains original values that must be converted
		** to fit inside tm object. For instance, in[Element::year] = 2012 yields
		** to 112 in pData structure
		**
		** \tparam An element of DateTuple
		*/
		template<std::size_t I>
		typename std::enable_if<I == std::tuple_size<DateTuple>::value, void>::type
			conversionHelper(const std::array<int, std::tuple_size<DateTuple>::value>& in);

		template<std::size_t I>
		typename std::enable_if<I < std::tuple_size<DateTuple>::value, void>::type
			conversionHelper(const std::array<int, std::tuple_size<DateTuple>::value>& in);

		enum { dateTupleSize =  std::tuple_size<DateTuple>::value };

		/*!
		** \brief Read the value, and assign it if different from -1
		**
		** \tparam An element of DateTuple
		**
		** \param[in, out] out An array with the values to consider for each element.
		** Basically, out[index(T)] = value
		** \param[in] value Value before conversion (for instance 2012 for T=Private::Year)
		** If -1, set #pIsElementPresent to false for the considered date element
		**
		** Note: Not used recursively
		*/
		template<class T>
		void constructorHelper(std::array<int, dateTupleSize>& out, int value);

		/*!
		** \brief Copy bitset informations in #copy methods
		*/
		template<class T>
		void copyBitsetHelper(const Date&);

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
	bool operator <= (const Date& lhs, const Date& rhs);
	void swap(Date& lhs, Date& rhs);

	//! Interpret the string dateRead and extract from it the date.
	bool dateFromExif(LoggingFacility& logs, Date& out, const YString& dateRead);

	//! Read the date and create the date as it would be written in exif file
	Yuni::CString<20, false> dateToExif(const Date& date);

} // namespace PictStock

# include "date.hxx"



#endif /* DATE_HPP_ */
