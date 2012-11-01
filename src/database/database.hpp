#ifndef __PICT_STOCK__DATABASE__DATABASE_HPP_
# define __PICT_STOCK__DATABASE__DATABASE_HPP_

# include <memory>
# include <yuni/core/string.h>
# include "../tools/sqlite_wrapper.hpp"
# include "cameras.hpp"
# include "photographers.hpp"
# include "private/parameters.hpp"


namespace PictStock
{
namespace Database
{

    class YUNI_DECL Database
    {
    public:

        //! Constructors
        //@{
        explicit Database(const AnyString& db3File, nsTable::Values mode = nsTable::load);


        //@}

        // SG DEV
        GenericTools::SqliteWrapper& sqliteDb() const
        {
            assert(pSqliteDb);
            return *pSqliteDb;
        }


        /*!
        ** \brief Open a sqlite file and fill accordingly all data
        */
        void open(const AnyString& db3File);

        //! Add a new photographer
        inline void addNewPhotographer(const TablePhotographers::FirstName::WrappedType& firstName,
            const TablePhotographers::LastName::WrappedType& lastName,
            const TablePhotographers::Abbreviation::WrappedType& abbreviation);

        /*!
        ** \brief Identify the owner of a photographer
        **
        ** If no photographer is identified, an exception is thrown:
        ** this method should be called when a camera has been identified
        ** and an abbreviation returned; this abbreviation should exist
        ** in photographers database unless this one is malformed
        */
        template<class StringT>
        inline void identifyCameraOwner(
            const StringT& abbreviation,
            Photographer::Ptr& photographer) const;

        /*!
        ** \brief Returns the underlying #Cameras object
        */
        inline const Cameras& cameras() const;


        /*!
        ** \brief Check the database includes the expected tables
        */
        bool isValidFile() const;


        /*!
        ** \brief Create the schemas of the database
        */
        void create(const AnyString& db3File);

        //! Path of the sqlite file
        inline const YString& path() const;


    private:

        //! Underlying database
        std::unique_ptr<GenericTools::SqliteWrapper> pSqliteDb;

        //! List of all cameras (matching the database table)
        std::unique_ptr<Cameras> pCameras;

        //! List of all photographers (matching the database table)
        std::unique_ptr<Photographers> pPhotographers;

        //! List of generic parameters
        std::unique_ptr<Private::Parameters> pParameters;

        //! Path of the sqlite file
        YString pPath;

    };

} // namespace Database
} // namespace PictStock

# include "database.hxx"

#endif /* __PICT_STOCK__DATABASE__DATABASE_HPP_ */
