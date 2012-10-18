#include "photographers.hpp"
#include "../tools/sqlite_wrapper.hpp"


namespace PictStock
{
namespace ExtendedPhoto
{

	using namespace TablePhotographers;

	Photographers::Photographers(GenericTools::SqliteWrapper& database)
		: pDatabase(database)
	{

		{
			// Request from the database
			YString command;
			{
				std::vector<YString> fields;
				Private::TupleFields<Tuple>::FieldNames(fields);
				std::for_each(fields.begin(), fields.end(), [&command](const YString& elt)
				{
					command << elt << ',';
				});
				command.removeLast();
				command << " FROM " << TableName() << " ORDER BY " << Abbreviation::FieldName() << ';';
			}

			{
				std::vector<TupleString> buf;
				database.select(buf, command);
				pData.reserve(buf.size());

				std::for_each(buf.begin(), buf.end(), [&pData](const TupleString& tuple)
					{
						pData.push_back(new Private::Photographer(tuple));
					}
				);
			}

		}
	}


	void Photographers::addNewPhotographer(const FirstName::StringType& firstName,
		const LastName::StringType& lastName,
		const Abbreviation::StringType& abbreviation)
	{
		// Determine names of the fields in the database
		std::vector<YString> fieldNames;
		Private::TupleFields<Tuple>::FieldNames(fieldNames);

		// Create a tuple with new elements to introduce
		TupleString newTuple;
		std::get<GenericTools::IndexOf<FirstName, Tuple>::value>(newTuple) = firstName;
		std::get<GenericTools::IndexOf<LastName, Tuple>::value>(newTuple) = lastName;
		std::get<GenericTools::IndexOf<Abbreviation, Tuple>::value>(newTuple) = abbreviation;

		// Add the photographer in the database (which will check by the way if the insertion is legit or not)
		pDatabase.insertData(TableName(), fieldNames, newTuple);

	}


} // namespace ExtendedPhoto
} // namespace PictStock
