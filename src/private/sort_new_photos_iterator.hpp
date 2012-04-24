#ifndef SORT_NEW_PHOTOS_ITERATOR_HPP_
# define SORT_NEW_PHOTOS_ITERATOR_HPP_

# include <yuni/io/directory/iterator.h>
# include "../extended_photo.hpp"

namespace SgPhoto
{
namespace Private
{


	class SortNewPhotosIterator : public Yuni::IO::Directory::IIterator<true>
	{
	public:

		//! Flow
		typedef Yuni::IO::Flow Flow;


	public:

		//! Constructor & destructor
		//@{

		/*!
		 * Constructor
		 *
		 * \param[in, out] logs Logging facility
		 * \param[in] inputDirectory All JPG files in this folder (or subfolders) will be sort
		 * \param[in] doFolderManuallyDate If true, when each folder is scanned there is a question
		 * to ask whether the user want to determine the date itself, so that the photo Exif
		 * might be corrected later if the date inside is wrong. This facility is useful when
		 * sorting old photos for which date of taking is quite unsure but which folder gives
		 * this information, albeit not necessarily in the chosen form. To put in the nutshell,
		 * should be false most of the time...
		 */
		explicit SortNewPhotosIterator(LoggingFacility& logs,
			const Yuni::String& inputDirectory, bool doFolderManualDate = false);

		//! Destructor
		virtual ~SortNewPhotosIterator();
		//@}

		/*!
		 * \brief Return the list of all pictures sort by date
		 *
		 * \param[out] out PicturesToProcess Key is date under format YYYYMMDD,
		 * value if a list of #ExtendedPhoto pointers
		 */
		void picturesToProcess(std::map<DateString, ExtendedPhoto::Vector>& out) const;

	public:

		//! Logs
		LoggingFacility& logs;



	private:

		//! Overload IIterator methods
		//@{
		virtual bool onStart(const Yuni::String& rootFolder);

		virtual Flow onBeginFolder(const Yuni::String&, const Yuni::String&, const Yuni::String& name);

		virtual void onEndFolder(const Yuni::String&, const Yuni::String&, const Yuni::String&);

		virtual Flow onFile(const Yuni::String&, const Yuni::String& folder, const Yuni::String& name, Yuni::uint64 size);

		virtual void onTerminate();
		//@}

	private:

		//! List of all jpeg files to process, sort by date
		std::map<DateString, ExtendedPhoto::Vector> pPicturesToProcess;

		//!

		/*!
		 * \brief Whether date might be modified manually or not while reading folders
		 *
		 * If true, when each folder is scanned there is a question
		 * to ask whether the user want to determine the date itself, so that the photo Exif
		 * might be corrected later if the date inside is wrong. This facility is useful when
		 * sorting old photos for which date of taking is quite unsure but which folder gives
		 * this information, albeit not necessarily in the chosen form. To put in the nutshell,
		 * should be false most of the time...
		 */
		const bool pDoFolderManualDate;

		/*!
		 * \brief Current manual date entrie in case #pDoFolderManualDate is true and date
		 * has been set manually.
		 *
		 * Empty otherwise
		 */
		DateString pCurrentFolderManualDate;



	};
} // namespace Private
} // namespace SgPhoto

# include "sort_new_photos_iterator.hxx"

#endif /* SORT_NEW_PHOTOS_ITERATOR_HPP_ */
