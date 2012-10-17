
#ifndef PATH_FORMAT_HELPERS_HPP_
# define PATH_FORMAT_HELPERS_HPP_

# include <map>
# include <yuni/core/string.h>
# ifdef USE_BOOST_REGULAR_EXPR
#  include <boost/regex.hpp>
# else // USE_BOOST_REGULAR_EXPR
#  include <regex>
# endif // USE_BOOST_REGULAR_EXPR
# include "../../pict_stock.hpp"
#include "../../extended_photo/path_informations.hpp"


namespace PictStock
{
	namespace ExtendedPhoto
	{
		// Forward declaration
		class ExtendedPhoto;
	} // namespace ExtendedPhoto

namespace PhotoDirectory
{
namespace Private
{

	/*!
	** \brief Class in charge of handling the model of output format provided in input parameters
	*/
	class YUNI_DECL PathFormatHelper : private Yuni::NonCopyable<PathFormatHelper>
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
		bool isOk(const AnyString& path, ExtendedPhoto::PathInformations& out) const;


		/*!
		** \brief Given a date and a photographer, generate the default output path matching it
		**
		** It is the minimal choice: many other expressions would also match the date and photographer
		** but we choose the minimal one
		*/
		void determineMinimalPath(Yuni::String& out,
			const ExtendedPhoto::PathInformations& infos) const;


		/*!
		** \brief Given a date and a photographer, generate the default output path matching it
		**
		** It is the minimal choice: many other expressions would also match the date and photographer
		** but we choose the minimal one
		*/
		void determineMinimalPath(Yuni::String& out,
			const ExtendedPhoto::ExtendedPhoto& photo) const;

		/*!
		** \brief Create a new RelevantInformations object featuring only informations useful
		** to determine the path
		**
		** For instance, if the folder part of a path contains only year and month,
		** returned object will have these values filled and all others set to
		** empty string
		**
		** \param[in] input The original #ExtendedPhoto::PathInformations object
		 */
		void onlyUsefulElements(ExtendedPhoto::PathInformations& out,
			const ExtendedPhoto::PathInformations& input) const;


		/*!
		** \brief Create a new RelevantInformations object featuring only informations useful
		** to determine the path
		**
		** For instance, if the folder part of a path contains only year and month,
		** returned object will have these values filled and all others set to
		** empty string
		**
		** \param[in] input #ExtendedPhoto object
		 */
		void onlyUsefulElements(ExtendedPhoto::PathInformations& out,
			const ExtendedPhoto::ExtendedPhoto& input) const;


	public:

		//! Logs
		LoggingFacility& logs;

		typedef std::map<unsigned int, unsigned int> MatchingType;



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
		**  \brief Container which tells the index to match an element from a regex_match
		**
		**  For instance, if Month was found in the format checkout out and the user-defined
		**  format is %P/%y/%m/%d
		**  pMatching will store the pair (2, 3) where 2 is the index of month in #Private::TupleList
		**  and 3 is the index you need to provide to the regex object to obtain the associated value
		**  (indexing begins at 1 for these objects)
		*/
		MatchingType pMatching;

	};


} // namespace Private
} // namespace PhotoDirectory
} // namespace PictStock


#endif /* PATH_FORMAT_HELPERS_HPP_ */
