#include "sqlite_wrapper_exceptions.hpp"

namespace
{
    static YString msgIncorrectPrepCommand(const AnyString& command)
    {
        YString msg("The preparation of the command \"");
        msg << command << "\" didn't return SQLITE_OK as expected";
        return std::move(msg);
    }

    static YString msgIncorrectRequest(const AnyString& request)
    {
        YString msg("A problem occurred with the request:\n\t");
        msg << request;
        return std::move(msg);
    }
} // namespace Anonymous


namespace GenericTools
{
namespace Exceptions
{

    SqliteWrapper::SqliteWrapper(const AnyString& msg)
        : Exception(msg)
    { }


    IncorrectPrepCommand::IncorrectPrepCommand(const AnyString& command)
        : SqliteWrapper(msgIncorrectPrepCommand(command))
    { }


    IncorrectRequest::IncorrectRequest(const AnyString& request)
        : SqliteWrapper(msgIncorrectRequest(request))
    { }



} // namespace Exceptions
} // namespace GenericTools
