#ifndef PATH_FORMAT_HPP_
# define PATH_FORMAT_HPP_

# include <yuni/core/string.h>
# ifdef USE_BOOST_REGULAR_EXPR
#  include <boost/regex.hpp>
# endif // USE_BOOST_REGULAR_EXPR
# include "../pict_stock.hpp"
# include "date.hpp"
# include "element.hpp"

namespace PictStock
{
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





	/*!
	** \brief Class in charge of handling the model of output format provided in input parameters
	*/
	class YUNI_DECL PathFormat
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
		explicit PathFormat(LoggingFacility& logs, const AnyString& format);

		//@}


		bool isOk(const AnyString& path, boost::cmatch& m) const;


	public:

		//! Logs
		mutable LoggingFacility& logs;


	private:

		/*!
		** \brief Set #pSymbolOrdering and #pRegExFolder
		**
		*/
		void setRegExFolder(const YString& path);

	private:

		#ifdef USE_BOOST_REGULAR_EXPR
		//! Regex formed from the input format; filename itself is not considered here
		boost::regex pRegExFolder;
		#endif // USE_BOOST_REGULAR_EXPR

		/*!
		** \brief Vector in which are stored the elements found in uqser-defined
		** expression in the correct order
		**
		** First entry is deliberately left with nullptr: in regex match the first entry
		** is always the entire expression, and I'd rather avoid using +1 everywhere
		** to match indexing of vector and indexing of match_result
		**
		*/
		Element::Vector pSymbolOrdering;


		//! List of all elements that might be used in regex expression (day, month, photographer, etc...)
		static const Element::Vector pElements;


		//! Expected filename format
		YString pFilenameFormat;
	};

}// namespace Private
}// namespace PictStock

#endif /* PATH_FORMAT_HPP_ */
