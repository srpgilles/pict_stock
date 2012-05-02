#ifndef PHOTO_DIRECTORY_HPP_
# define PHOTO_DIRECTORY_HPP_

# include <map>
# include <list>

# include <yuni/io/io.h>

# include "pict_stock.hpp"
# include "private/photo_directory_iterator.hpp"

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
		 */
		explicit PhotoDirectory(LoggingFacility& logs, Yuni::String pFolder);

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
