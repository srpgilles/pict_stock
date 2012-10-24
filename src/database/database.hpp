#ifndef PICT_STOCK_DATABASE_HPP
# define PICT_STOCK_DATABASE_HPP

# include <memory>
# include <yuni/core/string.h>
# include "../tools/sqlite_wrapper.hpp"
# include "cameras.hpp"
# include "photographers.hpp"


namespace PictStock
{
namespace Database
{

    class YUNI_DECL Database
    {
    public:

        //! Constructors
        //@{
        explicit Database(const AnyString& db3File);


        //@}


        /*!
        ** \brief Open a sqlite file and fill accordingly all data
        */
        void open(const AnyString& db3File);

        //! Add a new photographer
        inline void addNewPhotographer(const TablePhotographers::FirstName::WrappedType& firstName,
            const TablePhotographers::LastName::WrappedType& lastName,
            const TablePhotographers::Abbreviation::WrappedType& abbreviation);


    private:

        //! Underlying database
        std::unique_ptr<GenericTools::SqliteWrapper> pSqliteDb;

        //! List of all cameras (matching the database table)
        std::unique_ptr<Cameras> pCameras;

        //! List of all photographers (matching the database table)
        std::unique_ptr<Photographers> pPhotographers;

    };

} // namespace Database
} // namespace PictStock

# include "database.hxx"

#endif // PICT_STOCK_DATABASE_HPP
