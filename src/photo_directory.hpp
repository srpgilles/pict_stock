#ifndef PHOTO_DIRECTORY_HPP_
#define PHOTO_DIRECTORY_HPP_

#include <unordered_map>
#include <list>

#include <yuni/io/directory/iterator.h>
#include <yuni/io/io.h>


namespace SgPhoto
{

	typedef std::map<Yuni::String, std::list<Yuni::String> > ValidDirectoriesType;

	/*!
	 * \brief The class in charge of managing in memory the content of
	 * the output photo directory
	 *
	 */
	class PhotoDirectory
	{
	public:
		//! Constructor & destructor
		//@{
		/*!
		 * \brief Constructor
		 *
		 * \param pFolder Folder in which photos are sort
		 */
		explicit PhotoDirectory(Yuni::String pFolder);

		//! Destructor
		~PhotoDirectory();

		//@}

		//! Return the list of valid directories, sort by date
		// FOR DEBUG PURPOSES ONLY!
		const ValidDirectoriesType tree() const;

	private:

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
