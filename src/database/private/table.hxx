#ifndef TABLE_HXX
# define TABLE_HXX

namespace PictStock
{
namespace Database
{
namespace Private
{

    inline const YString& Table::tableSchema() const
    {
        return pSchema;
    }


    inline const YString& Table::tableName() const
    {
        return pName;
    }


    inline void Table::create(GenericTools::SqliteWrapper& sqlite)
    {
        sqlite.createTable(pName, pSchema);
    }


} // namespace Private
} // namespace Database
} // namespace PictStock


#endif // TABLE_HXX
