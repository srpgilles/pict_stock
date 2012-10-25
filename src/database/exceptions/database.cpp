#include "database.hpp"

namespace
{
    static YString msgFileNotFound(const AnyString& db3File)
    {
        YString msg("File ");
        msg << db3File << " doesn't exist";
        return std::move(msg);
    }

    static YString msgInvalidFile(const AnyString& db3File)
    {
        YString msg("File ");
        msg << db3File << " exist but does not respect expected database structure";
        return std::move(msg);
    }

} // namespace Anonymous


namespace PictStock
{
namespace Database
{
namespace Exceptions
{

    DatabaseException::DatabaseException(const AnyString& msg)
        : GenericTools::Exception(msg)
    { }


    FileNotFound::FileNotFound(const AnyString& db3File)
        : DatabaseException(msgFileNotFound(db3File))
    { }


    InvalidFile::InvalidFile(const AnyString& db3File)
        : DatabaseException(msgInvalidFile(db3File))
    { }



} // namespace Exception
} // namespace Database
} // namespace PictStock
