#ifndef PHOTO_DIRECTORY_ITERATOR_HPP_
# define PHOTO_DIRECTORY_ITERATOR_HPP_

# include <yuni/io/directory/iterator.h>
# include <yuni/core/string.h>

# include "../sg_photo.hpp"
# include "tools.hpp"


namespace SgPhoto
{
namespace Private
{

	/*!
	 * \brief A derived class from Yuni::IO::Directory::IIterator in charge
	 * of scanning the directory in which photos are to be stored
	 *
	 * The point is to determine which days are already present in this directory
	 * and for each day the associated path(s)
	 *
	 */
	class PhotoDirectoryIterator : public Yuni::IO::Directory::IIterator<true>
	{

	public:

		//! Constructor & destructor
		//@{
		//! Constructor
		PhotoDirectoryIterator(LoggingFacility& logs);

		//! Destructor
		virtual ~PhotoDirectoryIterator();

		//@}

		//! Flow
		typedef Yuni::IO::Flow Flow;

		/*!
		** \brief Extract the list of valid directories found
		**
		** \param[out] list List of valid directories.
		** Key is YYYYMMDD, value is the list of paths associated
		** to this date
		*/
		void validDirectories(ValidDirectoriesType& list);


	public:

		//! Logging facility
		mutable LoggingFacility& logs;

	private:
		//! Overload IIterator methods
		//@{
		virtual bool onStart(const Yuni::String& rootFolder);

		virtual Flow onBeginFolder(const Yuni::String& completePath,
			const Yuni::String&, const Yuni::String& name);

		virtual void onEndFolder(const Yuni::String&, const Yuni::String&, const Yuni::String&);

		virtual Flow onFile(const Yuni::String&, const Yuni::String& folder,
			const Yuni::String& name, Yuni::uint64 size);

		virtual void onTerminate();
		//@}


	 private:

		/*!
		** \brief Check the directory is valid, ie there are subfolders for
		** year, month and day
		*/
		bool checkValidity(const Yuni::String& directory);

	 private:

		unsigned int pCounter;
		unsigned int pFolderCount;
		unsigned int pValidFolderCount;

		/*!
		** \brief List of valid directories found in photo directory
		**
		** Key is YYYYMMDD, value is the list of paths associated
		** to this date
		*/
		ValidDirectoriesType pValidDirectories;

	};

} // namespace Private
} // namespace SgPhoto


#endif /* PHOTO_DIRECTORY_ITERATOR_HPP_ */