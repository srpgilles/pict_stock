#ifndef SQLITE_WRAPPER_EXCEPTIONS_HPP
# define SQLITE_WRAPPER_EXCEPTIONS_HPP

# include "../exceptions.hpp"

namespace GenericTools
{
namespace Exceptions
{

    struct YUNI_DECL SqliteWrapper : public Exception
    {
        SqliteWrapper(const AnyString& msg);
    };


    struct YUNI_DECL IncorrectPrepCommand : public SqliteWrapper
    {
        IncorrectPrepCommand(const AnyString& command);
    };



    struct YUNI_DECL IncorrectRequest : public SqliteWrapper
    {
        IncorrectRequest(const AnyString& request);
    };


} // namespace Exceptions
} // namespace GenericTools



#endif // SQLITE_WRAPPER_EXCEPTIONS_HPP
