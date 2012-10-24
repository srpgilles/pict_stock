#ifndef SQLITE_WRAPPER_EXCEPTIONS_HPP
# define SQLITE_WRAPPER_EXCEPTIONS_HPP

# include "../exceptions.hpp"

namespace GenericTools
{
namespace Exceptions
{
    namespace
    {
        static YString msgIncorrectPrepCommand(const AnyString& command)
        {
            YString msg("The preparation of the command \"");
            msg << command << "\" didn't return SQLITE_OK as expected";
            return std::move(msg);
        }
    } // namespace Anonymous


    struct YUNI_DECL SqliteWrapper : public Exception
    {
        SqliteWrapper(const AnyString& msg)
            : Exception(msg)
        { }
    };


    struct YUNI_DECL IncorrectPrepCommand : public SqliteWrapper
    {
        IncorrectPrepCommand(const AnyString& command)
            : SqliteWrapper(msgIncorrectPrepCommand(command))
        { }
    };


} // namespace Exceptions
} // namespace GenericTools



#endif // SQLITE_WRAPPER_EXCEPTIONS_HPP
