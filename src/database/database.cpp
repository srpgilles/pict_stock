#include "database.hpp"
#include "database_exceptions.hpp"
#include <yuni/io/file.h>

using namespace Yuni;

namespace PictStock
{
namespace Database
{

    Database::Database(const AnyString& db3File)
    {
        if (IO::File::Exists(db3File))
            throw Exceptions::FileNotFound(db3File);

        std::unique_ptr<GenericTools::SqliteWrapper> ptr {
            new GenericTools::SqliteWrapper(db3File, SQLITE_OPEN_READWRITE)
        };




    }

} // namespace Database
} // namespace PictStock
