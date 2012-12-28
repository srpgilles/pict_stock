#include "database.hpp"
#include "exceptions/database.hpp"
#include <yuni/io/file.h>
#include <set>

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
          pPhotographers(nullptr),
          pParameters(nullptr),
          pPath(db3File)
    {
        switch(mode)
        {
        case nsTable::load:
            open(db3File);
            break;
        case nsTable::createAndLoad:
            create(db3File);
            break;
        }
    }


    void Database::open(const AnyString& db3File)
    {
		YString foo;
		IO::MakeAbsolute(foo, db3File, false);

        if (!IO::File::Exists(foo))
            throw Exceptions::FileNotFound(foo);

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

        {
            std::unique_ptr<Private::Parameters> ptr(new Private::Parameters(sqliteRef));
            pParameters = std::move(ptr);
        }

        pPath = db3File;
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
            assert(IO::File::Exists(db3File));
            std::unique_ptr<Photographers> ptr(new Photographers(sqliteRef, nsTable::createAndLoad));
            pPhotographers = std::move(ptr);
        }

        assert(pPhotographers);
        auto& photographersRef = *pPhotographers;

        {
            std::unique_ptr<Cameras> ptr(new Cameras(sqliteRef, photographersRef, nsTable::createAndLoad));
            pCameras = std::move(ptr);
        }

        {
            std::unique_ptr<Private::Parameters> ptr(new Private::Parameters(sqliteRef, nsTable::createAndLoad));
            pParameters = std::move(ptr);
        }

        pPath = db3File;
    }


    bool Database::isValidFile() const
    {
        assert(pSqliteDb);
        std::set<YString> dbSchemas;
        pSqliteDb->schemas(dbSchemas);

        std::set<YString> expectedSchemas;
        expectedSchemas.insert(formSchemaStatement<Schema::Cameras>());
        expectedSchemas.insert(formSchemaStatement<Schema::Photographers>());
		expectedSchemas.insert(formSchemaStatement<Private::Schema::Parameters>());

        #if 0 // For debug when wrong db3 format
        std::cout << "Sqlite file:\n";
        GenericTools::printContainer(std::cout, dbSchemas,"\n");
        std::cout << "Expected schema:\n";
        GenericTools::printContainer(std::cout, expectedSchemas,"\n");
        #endif // 0

        return expectedSchemas == dbSchemas;
    }


} // namespace Database
} // namespace PictStock
