#ifndef __PICT_STOCK__PICT_FRAME__PRIVATE__SCAN_PHOTO_DIRECTORY_HPP_
# define __PICT_STOCK__PICT_FRAME__PRIVATE__SCAN_PHOTO_DIRECTORY_HPP_

# include <yuni/io/directory/iterator.h>
# include <yuni/core/string.h>
# include <deque>

# include "../../pict_stock.hpp"
# include "../../extended_photo/date.hpp"


namespace PictStock
{
	// Forward declarations
	namespace PhotoDirectory
	{
		class PathFormat;
	}

    namespace Database
	{
		class Database;
	}

namespace PictFrame
{

	// TODO Enum class will be better when supported by MSVC...
	namespace ReadDate
	{
		/*!
		** \brief Mode used to scan the date
		**
		** fast: rely only on folder name; is enough if folder part includes
		** the date and if the folder was properly generated by PictStock
		** safe: rely also on folder name, but inside a folder date of each
		** photo is checked as well
		**
		** TODO: Choose automatically the mode with respect of pathFormat
		*/


		enum Mode
		{
			fast,
			safe
		};
	} // namespace ReadDate



namespace Private
{

	/*!
	 * \brief A derived class from Yuni::IO::Directory::IIterator in charge
	 * of scanning the photo directory and take all photos that meet the entry criteria
	 * (begin and end date at the moment)
	 *
	 * NOTE: At the moment valid photos are stored in a deque; another possibility
	 * could be to iterate a second time (both should be tested to determine the
	 * most effective way to proceed)
	 *
	 */
	class YUNI_DECL ScanPhotoDirectory : public Yuni::IO::Directory::IIterator<false>,
										 private Yuni::NonCopyable<ScanPhotoDirectory>
	{

	public:

		//! Constructor & destructor
		//@{
		//! Constructor
		ScanPhotoDirectory(LoggingFacility& logs,
			const Database::Database& database,
			const PhotoDirectory::PathFormat& pathFormat,
			const YString& photoDirectory,
			const time_t beginDate, const time_t endDate, ReadDate::Mode mode);

		//! Destructor
		virtual ~ScanPhotoDirectory();

		//@}


		//! List of all photos that might be chosen.
		const std::deque<YString>& pool() const;

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
		ExtendedPhoto::Date pBeginDate;

		//! Newest date considered
		ExtendedPhoto::Date pEndDate;

		//! Mode
		ReadDate::Mode pMode;

		//! Object in charge of checking whether a directory is legit or not
		const PhotoDirectory::PathFormat& pPathFormat;

		//! List of all photos that might be chosen.
		std::deque<YString> pValidPhotos;

		//! Whether the current folder is valid or not
		bool pIsValidFolder;

		//! Object that keeps all known cameras under check
		const Database::Database& pDatabase;

	};

} // namespace Private
} // namespace PictFrame
} // namespace PictStock


#endif /* __PICT_STOCK__PICT_FRAME__PRIVATE__SCAN_PHOTO_DIRECTORY_HPP_ */
