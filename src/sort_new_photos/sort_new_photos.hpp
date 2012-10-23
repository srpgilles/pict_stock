#ifndef SORT_NEW_PHOTOS_HPP
# define SORT_NEW_PHOTOS_HPP

# include "../extended_photo/extended_photo.hpp"
# include "../photo_directory/photo_directory.hpp"
# include "private/sort_new_photos_iterator.hpp"
# include "../photo_directory/path_format.hpp"

namespace PictStock
{
	namespace ExtendedPhoto
	{
		class Cameras;
	}

namespace SortNewPhotos
{


    class YUNI_DECL SortNewPhotos : private Yuni::NonCopyable<SortNewPhotos>
    {


    public:

        //! Constructor & destructor
        //@{

        /*!
         * Constructor
         *
         * \param[in] inputDirectory All JPG files in this folder (or subfolders) will be sort
         * \param[in, out] photoDirectory Object in charge of maintaining the output folders
         * \param[in] doFolderManuallyDate If true, when each folder is scanned there is a question
		 * to ask whether the user want to determine the date itself, so that the photo Exif
		 * might be corrected later if the date inside is wrong. This facility is useful when
		 * sorting old photos for which date of taking is quite unsure but which folder gives
		 * this information, albeit not necessarily in the chosen form. To put in the nutshell,
		 * should be false most of the time...
		 * \param[in] summaryFile File in which all operations will be saved
		 */
        explicit SortNewPhotos(LoggingFacility& logs,
        	const Database::Cameras& cameras,
        	const Yuni::String& inputDirectory,
        	PhotoDirectory::PhotoDirectory& photoDirectory,
        	const YString& summaryFile,
        	bool doFolderManuallyDate = false);

        //! Destructor
        virtual ~SortNewPhotos();
        //@}

        //! Perform the actual sorting
        bool proceed();

    public:

        //! Logs
        LoggingFacility& logs;

    private:

        //! Object in charge of maintaining the output folders
        PhotoDirectory::PhotoDirectory& pPhotoDirectory;

        /*!
        ** Directory in which all JPG files are to be scanned and sort in the
        ** righteous #pPhotoDirectory
        */
        Yuni::String pInputDirectory;

        //! List of all jpeg files to process, sort by date
        ExtendedPhoto::OrderedPhotos pPicturesToProcess;

        //! Path to the file in which all operations are recorded
        const YString pSummaryFile;

        //! Object that keeps all known cameras under check
        const Database::Cameras& pCameras;

    };//! Path to the file in which all operations are recorded

} // namespace SortNewPhotos
} // namespace PictStock


#endif	/* SORT_NEW_PHOTOS_HPP */

