#ifndef PATH_FORMAT_HPP_
# define PATH_FORMAT_HPP_

# include <map>
# include <yuni/core/string.h>
# ifdef USE_BOOST_REGULAR_EXPR
#  include <boost/regex.hpp>
# else // USE_BOOST_REGULAR_EXPR
#  include <regex>
# endif // USE_BOOST_REGULAR_EXPR
# include "../pict_stock.hpp"
//# include "../extended_photo/path_informations.hpp"
# include "private/path_format_helpers.hpp"


namespace PictStock
{
	// Forward declarations
	class ExtendedPhoto;
	class PathInformations;



	class YUNI_DECL PathFormat : private Yuni::NonCopyable<PathFormat>
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

		//! Smart pointer most adapted for the class
		typedef Yuni::SmartPtr<PathFormat> Ptr;

		/*!
		** \brief Check whether the folder part of a path complies with the user-defined path format
		**
		** \param[in] path Path being checked
		** \param[out] out #RelevantInformations object in which only relevant fields have been completed
		*/
		bool doFolderMatch(const AnyString& path, PathInformations& out) const;

		/*!
		** \brief Given a date and a photographer, generate the default output path matching it
		**
		** It is the minimal choice: many other expressions would also match the date and photographer
		** but we choose the minimal one
		*/
		void determineMinimalFolder(Yuni::String& out, const YString& mainFolder, const ExtendedPhoto& photo) const;

		/*!
		** \brief Given a date and a photographer, generate the default output path matching it
		**
		** It is the minimal choice: many other expressions would also match the date and photographer
		** but we choose the minimal one
		*/
		void determineMinimalFolder(Yuni::String& out, const YString& mainFolder, const PathInformations& infos) const;


		/*!
		** \brief Given a date and a photographer, generate the default filename
		**
		** There are no numbers or .jpg extension: these are added in higher level classes
		**
		*/
		void determineMinimalFilename(Yuni::String& out, const ExtendedPhoto& photo) const;


		/*!
		** \brief Create a new RelevantInformations object featuring only informations useful
		** to determine the folder part of the user-defined format
		**
		** For instance, if the folder part of a path contains only year and month,
		** returned object will have these values filled and all others set to
		** empty string
		**
		** \param[in] input An #ExtendedPhoto object
		 */
		PathInformations onlyUsefulFolderElements(const ExtendedPhoto& input) const;

		/*!
		** \brief Create a new RelevantInformations object featuring only informations useful
		** to determine the folder part of the user-defined format
		**
		** For instance, if the folder part of a path contains only year and month,
		** returned object will have these values filled and all others set to
		** empty string
		**
		** \param[in] input The original #PathInformations object
		 */
		PathInformations onlyUsefulFolderElements(const PathInformations& input) const;


	public:

		LoggingFacility& logs;

	private:

		//! Manage the part of path before the filename
		Private::PathFormatHelper::Ptr pFolderPart;

		//! Manage the filename only, without considering the folder
		Private::PathFormatHelper::Ptr pFilePart;
	};


}// namespace PictStock

#endif /* PATH_FORMAT_HPP_ */
