#ifndef __PICT_STOCK__TOOLS__EXCEPTIONS__SQLITE_WRAPPER_EXCEPTIONS_HPP_
# define __PICT_STOCK__TOOLS__EXCEPTIONS__SQLITE_WRAPPER_EXCEPTIONS_HPP_

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



#endif /* __PICT_STOCK__TOOLS__EXCEPTIONS__SQLITE_WRAPPER_EXCEPTIONS_HPP_ */
