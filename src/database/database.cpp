#include "database.hpp"
#include "database_exceptions.hpp"
#include <yuni/io/file.h>

using namespace Yuni;

namespace PictStock
{
namespace Database
{

    Database::Database(const AnyString& db3File, nsTable::Values mode)
        : pSqliteDb(nullptr),
          pCameras(nullptr),
          pPhotographers(nullptr)
    {
        switch(mode)
        {
        case nsTable::Values::load:
            open(db3File);
            break;
        case nsTable::Values::createAndLoad:
            create(db3File);
            break;
        }
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


    void Database::create(const AnyString& db3File)
    {
        // Create the db3 file
        assert(!IO::File::Exists(db3File));

        {
            std::unique_ptr<GenericTools::SqliteWrapper> ptr(
                new GenericTools::SqliteWrapper(db3File, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE));

            pSqliteDb = std::move(ptr);
        }

        // Create the tables inside
        assert(pSqliteDb);
        auto& sqliteRef = *pSqliteDb;

        {
            std::unique_ptr<Photographers> ptr(new Photographers(sqliteRef, nsTable::createAndLoad));
            pPhotographers = std::move(ptr);
        }

        assert(pPhotographers);
        auto& photographersRef = *pPhotographers;

        {
            std::unique_ptr<Cameras> ptr(new Cameras(sqliteRef, photographersRef, nsTable::createAndLoad));
            pCameras = std::move(ptr);
        }
    }


    void Database::checkExpectedFormat()
    {



    }


} // namespace Database
} // namespace PictStock
