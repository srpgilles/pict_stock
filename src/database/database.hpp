#ifndef PICT_STOCK_DATABASE_HPP
# define PICT_STOCK_DATABASE_HPP

# include <memory>
# include <yuni/core/string.h>
# include "../tools/sqlite_wrapper.hpp"


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

    private:

        //! Underlying database
        std::unique_ptr<GenericTools::SqliteWrapper> pSqliteDb;


    };

} // namespace Database
} // namespace PictStock

#endif // PICT_STOCK_DATABASE_HPP
