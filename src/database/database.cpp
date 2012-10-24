#include "database.hpp"
#include "database_exceptions.hpp"
#include <yuni/io/file.h>

using namespace Yuni;

namespace PictStock
{
namespace Database
{

    Database::Database(const AnyString& db3File)
        : pSqliteDb(nullptr),
          pCameras(nullptr),
          pPhotographers(nullptr)
    {
        open(db3File);
    }


    void Database::open(const AnyString& db3File)
    {
        if (IO::File::Exists(db3File))
            throw Exceptions::FileNotFound(db3File);

        {
            std::unique_ptr<GenericTools::SqliteWrapper> ptr(
                new GenericTools::SqliteWrapper(db3File, SQLITE_OPEN_READWRITE));

            pSqliteDb = std::move(ptr);
        }

        assert(pSqliteDb);
        auto& sqliteRef = *pSqliteDb;

        {
            std::unique_ptr<Photographers> ptr(new Photographers(sqliteRef));
            pPhotographers = std::move(ptr);
        }

        assert(pPhotographers);
        auto& photographersRef = *pPhotographers;

        {
            std::unique_ptr<Cameras> ptr(new Cameras(sqliteRef, photographersRef));
            pCameras = std::move(ptr);
        }

    }

} // namespace Database
} // namespace PictStock
