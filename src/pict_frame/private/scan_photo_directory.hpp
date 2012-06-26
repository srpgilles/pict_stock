#ifndef SCAN_PHOTO_DIRECTORY_HPP_
# define SCAN_PHOTO_DIRECTORY_HPP_

# include <yuni/io/directory/iterator.h>
# include <yuni/core/string.h>
# include <deque>

# include "../../pict_stock.hpp"
# include "../../extended_photo/date.hpp"


namespace PictStock
{
	// Forward declaration
	class PathFormat;

namespace Private
{

	/*!
	 * \brief A derived class from Yuni::IO::Directory::IIterator in charge
	 * of scanning the photo directory and take all photos that meet the entry criteria
	 * (begin and end date at the moment)
	 *
	 */
	class YUNI_DECL ScanPhotoDirectory : public Yuni::IO::Directory::IIterator<false>,
										 private Yuni::NonCopyable<ScanPhotoDirectory>
	{

	public:

		//! Constructor & destructor
		//@{
		//! Constructor
		ScanPhotoDirectory(LoggingFacility& logs, const PathFormat& pathFormat,
			const time_t beginDate, const time_t endDate);

		//! Destructor
		virtual ~ScanPhotoDirectory();

		//@}

	public:

		//! Logging facility
		LoggingFacility& logs;

	private:
		//! Overload IIterator methods
		//@{
		virtual bool onStart(const Yuni::String& rootFolder);

		virtual Yuni::IO::Flow onBeginFolder(const Yuni::String& completePath,
			const Yuni::String&, const Yuni::String& name);

		virtual void onEndFolder(const Yuni::String&, const Yuni::String&, const Yuni::String&);

		virtual Yuni::IO::Flow onFile(const Yuni::String&, const Yuni::String& folder,
			const Yuni::String& name, Yuni::uint64 size);

		virtual void onTerminate();
		//@}


	 private:

		/*!
		** \brief Check the directory is valid, ie there are subfolders for
		** year, month and day
		**/
		bool checkValidity(const YString& directory);


	 private:

		//! Oldest date considered
		Date pBeginDate;

		//! Newest date considered
		Date pEndDate;

		//! Object in charge of checking whether a directory is legit or not
		const PathFormat& pPathFormat;

		//! List of all photos that might be chosen.
		std::deque<YString> pValidPhotos;

	};

} // namespace Private
} // namespace PictStock


#endif /* PHOTO_DIRECTORY_ITERATOR_HPP_ */
