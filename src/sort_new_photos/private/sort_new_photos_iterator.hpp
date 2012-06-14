#ifndef SORT_NEW_PHOTOS_ITERATOR_HPP_
# define SORT_NEW_PHOTOS_ITERATOR_HPP_

# include <yuni/io/directory/iterator.h>
# include "../../extended_photo/extended_photo.hpp"

namespace PictStock
{

	//! Forward declaration
	class PathFormat;

namespace Private
{

	class YUNI_DECL SortNewPhotosIterator : public Yuni::IO::Directory::IIterator<false>,
	 	 	 	 	 	 	 	 	 	    private Yuni::NonCopyable<SortNewPhotosIterator>
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
			const Yuni::String& inputDirectory, const PathFormat& pathFormat,
			bool doFolderManualDate = false);

		//! Destructor
		virtual ~SortNewPhotosIterator();
		//@}

		/*!
		 * \brief Return the list of all pictures sort by relevant informations
		 *
		 * \param[out] out PicturesToProcess Key is date under format YYYYMMDD,
		 * value if a list of #ExtendedPhoto pointers
		 */
		void picturesToProcess(OrderedPhotos& out) const;

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

		/*!
		** \brief  In case #pDoFolderManualDate = true, ask the user whether they want to set
		** manually the date for a given folder
		**
		** (notwithstanding possible subfolders)
		*/
		void proposeSetManualDate(const YString& folderName);


	private:

		//!
		const PathFormat& pPathFormat;

		//! List of all jpeg files to process, sort by date
		OrderedPhotos pPicturesToProcess;

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
		 * \brief Current manual date entry in case #pDoFolderManualDate is true and date
		 * has been set manually.
		 *
		 * Empty otherwise
		 */
		Yuni::CString<8, false> pCurrentFolderManualDate;

		/*!
		 * \brief Tells at which folder level we currently stand
		 *
		 * 0 is begin folder, 1 is one level above, and so on...
		 *
		 * For instance, {root folder}/foo/bar folder gets pFolderLevel = 2
		 */
		unsigned int pFolderLevel;

		/*!
		 * \brief If manual date applies also to subfolders, keep in memory the level to which
		 * folder at which the choice was made stands.
		 *
		 * For instance, if a target date has been set for {root folder}/foo, we keep
		 * #pCurrentFolderManual = 1 and current date will stands until we go into an entirely
		 * other branch of the tree (ie pFolderLevel must go back to 0)
		 */
		unsigned int pCurrentFolderManualLevel;



	};

} // namespace Private
} // namespace PictStock

# include "sort_new_photos_iterator.hxx"

#endif /* SORT_NEW_PHOTOS_ITERATOR_HPP_ */
