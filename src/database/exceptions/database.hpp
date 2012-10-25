#ifndef DATABASE_EXCEPTIONS_HPP
# define DATABASE_EXCEPTIONS_HPP

# include "../../tools/exceptions.hpp"


namespace PictStock
{
namespace Database
{
namespace Exceptions
{

    struct YUNI_DECL DatabaseException : public GenericTools::Exception
    {
        DatabaseException(const AnyString& msg);
    };


    struct YUNI_DECL FileNotFound : public DatabaseException
    {
        FileNotFound(const AnyString& db3File);
    };


    struct YUNI_DECL InvalidFile : public DatabaseException
    {
        InvalidFile(const AnyString& db3File);
    };



} // namespace Exception
} // namespace Database
} // namespace PictStock


#endif // DATABASE_EXCEPTIONS_HPP
