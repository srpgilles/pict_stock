#include "parameters.hpp"
#include "../../tools/sqlite_wrapper.hpp"


namespace PictStock
{
namespace Database
{
namespace Private
{

    using namespace TableParameters;

    Parameters::Parameters(GenericTools::SqliteWrapper& database, nsTable::Values mode)
        : Private::Table<Schema::Parameters>(),
          pDatabase(database)
	{
        switch(mode)
        {
            case nsTable::createAndLoad:
                create(database);
                load();
                break;
            case nsTable::load:
                load();
                break;
        }
    }


    void Parameters::load()
    {
        // Request from the database
        YString command;
        {
            std::vector<YString> fields;
            GenericTools::Tuple::Fields<Tuple>::FieldNames(fields);
            std::for_each(fields.begin(), fields.end(), [&command](const YString& elt)
            {
                command << elt << ',';
            });
            command.removeLast();
            command << " FROM " << Schema::Parameters::Name() << ';';
        }

        pDatabase.select(pRows, command);
	}

} // namespace Private
} // namespace Database
} // namespace PictStock
