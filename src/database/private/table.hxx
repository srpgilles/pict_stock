#ifndef __PICT_STOCK__DATABASE__PRIVATE__TABLE_HXX_
# define __PICT_STOCK__DATABASE__PRIVATE__TABLE_HXX_

namespace PictStock
{
namespace Database
{
namespace Private
{

    template<class T>
    Table<T>::Table()
        : pName(T::Name()),
          pSchema(T::Schema())
    { }


    template<class T>
    inline const YString& Table<T>::tableSchema() const
    {
        return pSchema;
    }


    template<class T>
    inline const YString& Table<T>::tableName() const
    {
        return pName;
    }


    template<class T>
    inline void Table<T>::create(GenericTools::SqliteWrapper& sqlite)
    {
        sqlite.createTable(pName, pSchema);
    }


} // namespace Private
} // namespace Database
} // namespace PictStock


#endif /* __PICT_STOCK__DATABASE__PRIVATE__TABLE_HXX_ */
