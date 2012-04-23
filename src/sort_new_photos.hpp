#ifndef SORT_NEW_PHOTOS_HPP
# define SORT_NEW_PHOTOS_HPP

# include "extended_photo.hpp"
# include "photo_directory.hpp"

namespace SgPhoto
{

    class SortNewPhotos : public Yuni::IO::Directory::IIterator<true>
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



    protected:

        //! Overload IIterator methods
        //@{
        virtual bool onStart(const Yuni::String& rootFolder);

        virtual Flow onBeginFolder(const Yuni::String&, const Yuni::String&, const Yuni::String& name);

        virtual void onEndFolder(const Yuni::String&, const Yuni::String&, const Yuni::String&);

        virtual Flow onFile(const Yuni::String&, const Yuni::String& folder, const Yuni::String& name, Yuni::uint64 size);

        virtual void onTerminate();
        //@}



    private:

        //! Visual check of valid directories
        void printValidDirectories(std::ostream& out) const;

    private:

        //! Object in charge of maintaining the output folders
        PhotoDirectory& pPhotoDirectory;

        /*!
         ** Directory in which all JPG files are to be scanned and sort in the
         ** righteous #pPhotoDirectory
         */
        Yuni::String pInputDirectory;


        /*!
         ** \brief A map container containing the automatic choices
         **
         ** When a photo is scanned, the program asks the user where he wants to store the photo,
         ** and whether the choice should be propagated to other photos. If so, use default path
         ** stored in following object
         **
         ** Key is the date, value is the automatic path
         */
        ValidDirectoriesType pAutomaticChoice;

    };

} // end namespace SgPhoto


#endif	/* SORT_NEW_PHOTOS_HPP */

