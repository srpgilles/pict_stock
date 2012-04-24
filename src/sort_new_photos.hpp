#ifndef SORT_NEW_PHOTOS_HPP
# define SORT_NEW_PHOTOS_HPP

# include "extended_photo.hpp"
# include "photo_directory.hpp"
# include "private/sort_new_photos_iterator.hpp"

namespace SgPhoto
{

    class SortNewPhotos
    {


    public:

        //! Constructor & destructor
        //@{

        /*!
         * Constructor
         *
         * \param[in] inputDirectory All JPG files in this folder (or subfolders) will be sort
         * \param[in, out] photoDirectory Object in charge of maintaining the output folders
         */
        explicit SortNewPhotos(LoggingFacility& logs, const Yuni::String& inputDirectory,
        	PhotoDirectory& photoDirectory);

        //! Destructor
        virtual ~SortNewPhotos();
        //@}

    public:

        //! Logs
        LoggingFacility& logs;

    private:

        //!
        bool proceed();

    private:

        //! Object in charge of maintaining the output folders
        PhotoDirectory& pPhotoDirectory;

        /*!
         ** Directory in which all JPG files are to be scanned and sort in the
         ** righteous #pPhotoDirectory
         */
        Yuni::String pInputDirectory;

        //! List of all jpeg files to process, sort by date
        std::map<DateString, ExtendedPhoto::Vector> pPicturesToProcess;

    };

} // end namespace SgPhoto


#endif	/* SORT_NEW_PHOTOS_HPP */

