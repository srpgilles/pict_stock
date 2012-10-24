#include "table.hpp"
#include "../../tools/sqlite_wrapper.hpp"

namespace PictStock
{
namespace Database
{
namespace Private
{

    Table::Table(const YString& name, const YString& schema)
        : pName(name),
          pSchema(schema)
    { }





} // namespace Private
} // namespace Database
} // namespace PictStock

