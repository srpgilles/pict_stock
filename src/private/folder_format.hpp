#ifndef FOLDER_FORMAT_HPP_
# define FOLDER_FORMAT_HPP_

#include <yuni/core/string.h>
#ifdef USE_BOOST_REGULAR_EXPR
#include <boost/regex.hpp>
#endif // USE_BOOST_REGULAR_EXPR


namespace PictStock
{
namespace Private
{

	/*!
	** \brief Class in charge of handling the model of output format provided in input parameters
	*/
	class YUNI_DECL FolderFormat
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
		** 			2009/04/02/Photo_14_05_CSG.jpg for a photo that would have been taken at that date
		** 			by photographer indexed by letter CSG (as a matter of fact this is class
		** 			#ExtendedPhoto which will be in charge of implementing this, not current one)
		 */
		explicit FolderFormat(const YString& format);

		//@}




	private:

		#ifdef USE_BOOST_REGULAR_EXPR
		//! Regex formed from the input format; filename itself is not considered here
		boost::regex pRegExFolder;
		#endif // USE_BOOST_REGULAR_EXPR



	};

}// namespace Private
}// namespace PictStock

#endif /* FOLDER_FORMAT_HPP_ */
