#include <yuni/core/getopt.h>
#include "database/database.hpp"

using namespace Yuni;
using namespace PictStock;


int main(int argc, char* argv[])
{
    (void) argc;
	(void) argv;

    Database::Database database("test.db3", Database::nsTable::load);
    auto& db = database.sqliteDb();
    typedef PictStock::Database::Cameras::Tuple Tuple;

	std::cout << std::tuple_size<Tuple>::value << '\n';
    typedef GenericTools::Tuple::WrappedType<Tuple>::type Strings;


	std::cout << std::tuple_size<Strings>::value << '\n';

    {
        std::vector<Strings> pRows;
        db.select(pRows, "Keyword,Value,Owner FROM Cameras ORDER BY Keyword");

        std::vector<YString> fieldNames;
        GenericTools::Tuple::Fields<Tuple>::FieldNames(fieldNames);

        for (auto it = fieldNames.cbegin(), end = fieldNames.cend(); it != end; ++it)
            std::cout << *it << '\n';
    }

	return 0;
}
