#ifndef PHOTO_DIRECTORY_HPP_
# define PHOTO_DIRECTORY_HPP_

# include <map>
# include <list>

# include <yuni/io/io.h>

# include "pict_stock.hpp"
# include "private/photo_directory_iterator.hpp"
# include "private/path_format.hpp"

namespace PictStock
{

	/*!
	 * \brief The class in charge of managing in memory the content of
	 * the output photo directory
	 *
	 */
	class YUNI_DECL PhotoDirectory
	{
	public:
		//! Constructor & destructor
		//@{
		/*!
		 * \brief Constructor
		 *
		 * \param pFolder Folder in which photos are sort
		 * \param[in] pathFormat Format given by the user; the rules are:
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
		 *
		 */
		explicit PhotoDirectory(LoggingFacility& logs, Yuni::String pFolder, const YString& pathFormat);

		//! Destructor
		~PhotoDirectory();

		//@}

		/*!
		 * \brief Check whether the date of the photo has already been handled or not
		 *
		 * \param[in] date Date in format YYYYMMDD
		 * \param[out] folders List of the folders associated to the date
		 *
		 * \return True if the date is known, false otherwise
		 */
		bool findDate(const DateString& date, std::list<YString>& folders) const;

		
		//! Print the valid folder founds (for debug purposes)
		void print(std::ostream& out) const;

		/*!
		 * \briefCreate the basic folder matching a peculiar date
		 *
		 *  \param[in] date Date for which the folde ris to be created
		 *  \param[out] folder Name of the folder created
		 *
		 *  \return True If creation went right
		 */
		bool createDateFolder(const DateString& date, YString& folder);


	public:

		//! Logging facility
		mutable LoggingFacility& logs;

	private:

		//! Main folder in which all photos are stored in the end
		YString pMainFolder;

		//! Object in charge of enforcing user-defined format
		Private::PathFormat::Ptr pPathFormat;

		/*!
		** \brief List of valid directories found in photo directory
		**
		** Key is YYYYMMDD, value is the list of paths associated
		** to this date
		*/
		ValidDirectoriesType pTree;
	};


} // namespace SgPĥoto

# include "photo_directory.hxx"

#endif /* PHOTO_DIRECTORY_HPP_ */
