#ifndef DATABASE_EXCEPTIONS_HPP
# define DATABASE_EXCEPTIONS_HPP

# include "../tools/exceptions.hpp"


namespace PictStock
{
namespace Database
{
namespace Exceptions
{
    namespace
    {
        static YString msgFileNotFound(const AnyString& db3File)
        {
            YString msg("File ");
            msg << db3File << " doesn't exist";
            return std::move(msg);
        }
    } // namespace Anonymous


    struct YUNI_DECL Exception : public GenericTools::Exception
    {
        Exception(const AnyString& msg)
            : GenericTools::Exception(msg)
        { }
    };


    struct YUNI_DECL FileNotFound : public Exception
    {
        FileNotFound(const AnyString& db3File)
            : Exception(msgFileNotFound(db3File))
        { }
    };



} // namespace Exception
} // namespace Database
} // namespace PictStock


#endif // DATABASE_EXCEPTIONS_HPP
