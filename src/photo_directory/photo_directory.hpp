#ifndef __PICT_STOCK__PHOTO_DIRECTORY__PHOTO_DIRECTORY_HPP_
# define __PICT_STOCK__PHOTO_DIRECTORY__PHOTO_DIRECTORY_HPP_

# include <map>
# include <list>

# include <yuni/io/io.h>

# include "../pict_stock.hpp"
# include "path_format.hpp"

namespace PictStock
{
    namespace Database
	{
		class Database;
	}

namespace PhotoDirectory
{

	/*!
	 * \brief The class in charge of managing in memory the content of
	 * the output photo directory
	 *
	 */
	class YUNI_DECL PhotoDirectory : private Yuni::NonCopyable<PhotoDirectory>
	{
	public:
		//! Constructor & destructor
		//@{
		/*!
		** \brief Constructor
		**
		** \param pFolder Folder in which photos are sort
		** \param[in] pathFormat Format given by the user; the rules are:
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
        explicit PhotoDirectory(LoggingFacility& logs, const Database::Database& database,
			const Yuni::String& pFolder, const YString& pathFormat);

		//! Destructor
		~PhotoDirectory();

		//@}


		/*!
		 * \brief Create the minimal basic folder matching a peculiar set of informations
		 * (date, photographer) and store it in the class data
		 *
		 *  \param[out] folder Name of the folder created
		 *
		 *  \return True If creation went right
		 */
		bool createFolder(YString& folder, const ExtendedPhoto::PathInformations& infos);


		//! Return the path format defined by the user
		inline PathFormat::Ptr pathFormat() const;

	public:

		//! Logging facility
		LoggingFacility& logs;


	private:

		//! Main folder in which all photos are stored in the end
		YString pMainFolder;

		//! Object in charge of enforcing user-defined format
		PathFormat::Ptr pPathFormat;

		/*!
		** \brief List of valid directories found in photo directory
		*/
		ValidDirectoriesType pTree;

		//! Object that keeps all known cameras under check
        const Database::Database& pDatabase;

	};


} // namespace PhotoDirectory
} // namespace PictStock

# include "photo_directory.hxx"

#endif /* __PICT_STOCK__PHOTO_DIRECTORY__PHOTO_DIRECTORY_HPP_ */
