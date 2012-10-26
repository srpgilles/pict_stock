#include "database.hpp"
#include "exceptions/database.hpp"
#include <yuni/io/file.h>

using namespace Yuni;

namespace PictStock
{
namespace Database
{
    namespace
    {
        /*!
        ** \brief Recreate the complete sql command that would be used to
        ** create anew the structure of the table
        */

        template<class TableT>
        YString formSchemaStatement()
        {
            YString ret;
            ret << "CREATE TABLE ";
            ret << TableT::Name();
            ret << '(' << TableT::Schema() << ')';
            return std::move(ret);
        }


    } // namespace anonymous


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
        if (!IO::File::Exists(db3File))
            throw Exceptions::FileNotFound(db3File);

        {
            std::unique_ptr<GenericTools::SqliteWrapper> ptr(
                new GenericTools::SqliteWrapper(db3File, SQLITE_OPEN_READWRITE));

            pSqliteDb = std::move(ptr);
        }

        assert(pSqliteDb);

        if (!isValidFile())
            throw Exceptions::InvalidFile(db3File);

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
        assert("The method should be called only when the file doesn't exist"
            && !IO::File::Exists(db3File));

        {            
            std::unique_ptr<GenericTools::SqliteWrapper> ptr(
                new GenericTools::SqliteWrapper(db3File, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE));

            pSqliteDb = std::move(ptr);
        }

        // Create the tables inside
        assert(pSqliteDb);
        auto& sqliteRef = *pSqliteDb;

        {
            assert(Yuni::IO::File::Exists(db3File));
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


    bool Database::isValidFile() const
    {
        assert(pSqliteDb);
        std::unordered_set<YString> dbSchemas;
        pSqliteDb->schemas(dbSchemas);

        std::unordered_set<YString> expectedSchemas;
        expectedSchemas.insert(formSchemaStatement<Schema::Cameras>());
        expectedSchemas.insert(formSchemaStatement<Schema::Photographers>());

        return expectedSchemas == dbSchemas;
    }


} // namespace Database
} // namespace PictStock
