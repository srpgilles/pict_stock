
#ifndef PATH_FORMAT_HELPERS_HPP_
# define PATH_FORMAT_HELPERS_HPP_

# include <map>
# include <bitset>
# include <yuni/core/string.h>
# ifdef USE_BOOST_REGULAR_EXPR
#  include <boost/regex.hpp>
# else // USE_BOOST_REGULAR_EXPR
#  include <regex>
# endif // USE_BOOST_REGULAR_EXPR
# include "../../pict_stock.hpp"
# include "traits/traits.hpp"
# include <tuple>


namespace PictStock
{
	// Forward declaration
	class ExtendedPhoto;

namespace Private
{
	class YUNI_DECL PathFormatException : public std::exception
	{
	public:

		//! Constructor & destructor
		//@{
		/*!
		** \brief Constructor
		**
		** \param msg Message describing the problem encountered
		*/
		PathFormatException(const AnyString& msg);

		virtual ~PathFormatException() throw();

		//@}

		//! Overwrite what method
		virtual const char* what() const throw();

	private:

		//! Text describing the exception
		YString pMessage;
	};


	typedef Yuni::CString<4, false> SymbolString;
	typedef Yuni::CString<30, true> RegexString;

	struct Year
	{
		static const SymbolString Symbol() { return "%y"; }
		static const RegexString Regex() { return "\\d{4}"; }
	};


	struct Month
	{
		static const SymbolString Symbol() { return "%m"; }
		static const RegexString Regex() { return " \\d{1}|0\\d{1}|10|11|12"; }
	};


	struct Day
	{
		static const SymbolString Symbol() { return "%d"; }
		static const RegexString Regex() { return  "[ |0|1|2]\\d{1}|3[0|1]"; } // no check for adequacy month day (for instance 30 Elements::b is allowed)
	};


	struct Hour
	{
		static const SymbolString Symbol() { return "%H"; }
		static const RegexString Regex() { return  "[ |0|1]\\d{1}|2[0-4]|  "; } // empty allowed
	};


	struct Minute
	{
		static const SymbolString Symbol() { return "%M"; }
		static const RegexString Regex() { return  "[ 0-5]\\d{1}|  "; } // empty is allowed
	};


	struct Second
	{
		static const SymbolString Symbol() { return "%S"; }
		static const RegexString Regex() { return  "[ 0-5]\\d{1}|  "; } // empty is allowed
	};


	struct Photographer
	{
		static const SymbolString Symbol() { return "%P"; }
		static const RegexString Regex() { return ".*"; }
	};


	/*!
	** \brief Class in charge of handling the model of output format provided in input parameters
	*/
	class YUNI_DECL PathFormatHelper
	{
	public:


		//! Constructor & destructor
		//@{
		/*!
		** \brief Constructor
		**
		** \param format Format given by the user; the rules are:
		** 		. folder separator is indicated as '/', whatever the system you are working on
		** 		. format must end with ".jpg" (case insensitive)
		** 		. special commands that will be inserted depending on the photo considered
		** 		These commands are:
		** 			%y for the year
		** 			%m for month
		** 			%d for day
		** 			%H for hour
		** 			%M for minutes
		** 			%S for seconds
		** 			%P for the photographer
		** 		An example:
		** 			%y/M%m/J%d/Photo_%H%M_%P.jpg
		** 			will lead to
		** 			2009/M04/J02/Photo_14_05_CSG.jpg for a photo that would have been taken at that date
		** 			by photographer indexed by letter CSG (as a matter of fact this is class
		** 			#ExtendedPhoto which will be in charge of implementing this, not current one)
		**
		** 		Wildcards such as * can also be used (as a matter of fact format is almost
		** 		a regular expression; only %x terms will be replaced by some more appropriates
		** 		to form a valid regular expression
		**
		** 		Please notice that I'd rather use Yuni::String substitution instead of regex one
		** 		as I am under the impress regex one would imply lots of unnecessary copy assignations
		** 		(lots of temporary strings would be defined). I guess it's useful in some cases,
		** 		but it seems rather overkill for my own purposes
		**
		 */
		explicit PathFormatHelper(LoggingFacility& logs, const AnyString& format);

		//@}

		//! Smart pointer most adapted for the class
		typedef Yuni::SmartPtr<PathFormatHelper> Ptr;


		/*!
		** \brief Check whether the folder part of a path complies with the user-defined path format
		**
		** \param[in] path Path being checked
		** \param[out] out #pathInformations object in which only path fields have been completed
		*/
		bool isOk(const AnyString& path, PathInformations& out) const;


		/*!
		** \brief Given a date and a photographer, generate the default output path matching it
		**
		** It is the minimal choice: many other expressions would also match the date and photographer
		** but we choose the minimal one
		*/
		void determineMinimalPath(Yuni::String& out,
			const PathInformations& infos) const;


		/*!
		** \brief Given a date and a photographer, generate the default output path matching it
		**
		** It is the minimal choice: many other expressions would also match the date and photographer
		** but we choose the minimal one
		*/
		void determineMinimalPath(Yuni::String& out,
			const ExtendedPhoto& photo) const;


	public:

		//! Logs
		mutable LoggingFacility& logs;

		//! Bitset to know whether a given element is in the folder path
		std::bitset<Elements::size> doContains;


	private:

		/*!
		** \brief Set #pSymbolOrdering and #pRegEx
		**
		*/
		void setRegEx();

		/*!
		** \brief Look in the user-defined format to find the positions of relevant informations
		**
		** All types in the tuple list will be tried out to check whether their symbol is present
		** in the user defined format. If so, a vector will be returned with the index of the
		** relevant symbols in the appropriate order.
		**
		** For instance, if format = "%d/%y", only year and day informations are considered
		** (and month ones for instance are ignored - I know it's a pretty stupid choice, but
		** I'll use this one for the sake of example)
		**
		** The output vector will feature the positions of year and day values in the tuple list.
		**
		** So for instance if list is { Year, Month, Day, Hour, Minute, Second }, the
		** output vector will return (2, 0)
		** meaning the first informations found is index 2 in the tuple list (namely day)
		** and second one is year
		*/
		void interpretUserDefinedFormat();


	private:

		//! User-defined path format
		YString pFormat;

		#ifdef USE_BOOST_REGULAR_EXPR
		//! Regex formed from the input format; filename itself is not considered here
		boost::regex pRegEx;
		#else
		std::regex pRegEx;
		#endif // USE_BOOST_REGULAR_EXPR

		/*!
		** \brief Vector in which are stored the elements found in user-defined
		** expression in the correct order
		**
		*/
		Traits::Element::Vector pSymbolOrdering;

		//! List of all elements that might be used in regex expression (day, month, photographer, etc...)
		static const Traits::Element::Vector pElements;

		//! Tuple of all possible relevant elements in the user-defined format
		typedef std::tuple<Year, Month, Day, Hour, Minute, Second, Photographer> TupleType;

		/*!
		** \brief Vector in charge of keeping the book to know which informations is stored where
		**
		** An example will value here thousands of explanations:
		**   if pPositions = {3, 1, 4}
		**
		**   it means fourth, second and fifth element of TupleType are encountered in this
		**   order in user-defined format (so Hour, Month and Minute) and others
		**   aren't seen at all
		*/
		std::vector<size_t> pPositions;

	};


} // namespace Private
} // namespace PictStock

# include "path_format_helpers.hxx"

#endif /* PATH_FORMAT_HELPERS_HPP_ */
