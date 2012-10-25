#ifndef TABLE_HPP
# define TABLE_HPP

# include <yuni/core/string.h>
# include "../../tools/sqlite_wrapper.hpp"

namespace GenericTools
{
    class SqliteWrapper;
}

namespace PictStock
{
namespace Database
{
    namespace nsTable // waiting for enum class support in MSVC
    {
        enum Values
        {
            createAndLoad,
            load
        };
    } // namespace nsTable

namespace Private
{

    /*!
    ** \brief All objects representing a database table should inherit from this class
    **
    ** T is a structure with the expected static members Name() and Schema() that defines
    ** respectively the name of the table and the creation command as passed to
    ** SqliteWrapper::createTable()
    */

    template<class T>
    class YUNI_DECL Table
    {
    public:

        //! Constructors
        //@{
        explicit Table();

        //@}

        //! Name of the table
        inline const YString& tableName() const;

        /*!
        ** \brief Schema used to create the table of the database
        **
        ** As given to GenericTools::SqliteWrapper::create()
        **
        ** For instance:
        **  "Keyword varchar(80),"
        **  "Value varchar(80),"
        **  "Owner varchar(8),"
        **  "FOREIGN KEY(Owner) REFERENCES Photographers(Abbreviation),"
        **  "UNIQUE (Keyword, Value) ON CONFLICT ABORT"
        **/
        inline const YString& tableSchema() const;

    protected:

        //! Create the table.
        inline void create(GenericTools::SqliteWrapper& sqlite);

    private:

        //! Name of the table
        YString pName;

        //! Schema used to create the table of the database
        YString pSchema;
    };

} // namespace Private
} // namespace Database
} // namespace PictStock

# include "table.hxx"



#endif // TABLE_HPP
