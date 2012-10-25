#include "photographers.hpp"
#include "../tools/sqlite_wrapper.hpp"


namespace PictStock
{
namespace Database
{
    using namespace TablePhotographers;

    Photographers::Photographers(GenericTools::SqliteWrapper& database, nsTable::Values mode)
        : Private::Table<PictStock::Database::Schema::Photographers>(),
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


    void Photographers::load()
    {

        YString command;
        {
            std::vector<YString> fields;
            GenericTools::Tuple::Fields<Tuple>::FieldNames(fields);
            std::for_each(fields.begin(), fields.end(), [&command](const YString& elt)
            {
                command << elt << ',';
            });
            command.removeLast();
            command << " FROM " << TableName() << " ORDER BY " << Abbreviation::FieldName() << ';';
        }

        {
            std::vector<TupleWrappedType> buf;
            pDatabase.select(buf, command);
            pData.reserve(buf.size());

            std::for_each(buf.begin(), buf.end(), [this](const TupleWrappedType& tuple)
                {
                    pData.push_back(new Photographer(tuple));
                }
            );
        }
	}


	void Photographers::addNewPhotographer(const FirstName::WrappedType& firstName,
		const LastName::WrappedType& lastName,
		const Abbreviation::WrappedType& abbreviation)
	{
		// Determine names of the fields in the database
		std::vector<YString> fieldNames;
        GenericTools::Tuple::Fields<Tuple>::FieldNames(fieldNames);

		// Create a tuple with new elements to introduce
		TupleWrappedType newTuple;
        std::get<GenericTools::Tuple::IndexOf<FirstName, Tuple>::value>(newTuple) = firstName;
        std::get<GenericTools::Tuple::IndexOf<LastName, Tuple>::value>(newTuple) = lastName;
        std::get<GenericTools::Tuple::IndexOf<Abbreviation, Tuple>::value>(newTuple) = abbreviation;

		// Add the photographer in the database (which will check by the way if the insertion is legit or not)
		pDatabase.insertData(TableName(), fieldNames, newTuple);

	}


	bool Photographers::findPhotographer(Photographer::Ptr& photographer,
		const TablePhotographers::Abbreviation::WrappedType& abbreviation) const
	{
		auto end = pData.cend();
		auto it = std::find_if(pData.cbegin(), end,
			[&abbreviation](const Photographer::Ptr& photographer) -> bool
			{
				return photographer->abbreviation() == abbreviation;
			}
		);

		if (it == end)
		{
			photographer = nullptr;
			return false;
		}

		photographer = *it;
		assert(!(!photographer));

		return true;
	}


} // namespace Database
} // namespace PictStock