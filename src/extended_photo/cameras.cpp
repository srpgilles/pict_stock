#include "cameras.hpp"
#include "../tools/sqlite_wrapper.hpp"



namespace PictStock
{
namespace ExtendedPhoto
{

	using namespace TableCameras;

	Cameras::Cameras(GenericTools::SqliteWrapper& database)
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
				command << " FROM " << TableName() << " ORDER BY Keyword";
			}

			database.select(pRows, command);
		}

		{
			// Also init #pKeywords
			typedef std::tuple<Keyword::StringType> TupleKeyword;
			std::vector<TupleKeyword> keywords;
			YString command("Keyword FROM ");
			command << TableName();
			database.select(keywords, command);

			for (auto it = keywords.begin(), end = keywords.end(); it != end; ++it)
				pKeywords.insert(std::get<0>(*it));
		}
	}


	bool Cameras::identifyPhotographer(
		const Keyword::StringType& currentKeyword,
		const Value::StringType& valueToCheck,
		Owner::StringType& photographer) const
	{
		enum { indexKeyword = GenericTools::IndexOf<Keyword, Tuple>::value };
		enum { indexValue = GenericTools::IndexOf<Value, Tuple>::value };

		TupleString lookedAt;
		std::get<indexKeyword>(lookedAt) = currentKeyword;
		std::get<indexValue>(lookedAt) = valueToCheck;

		// Equal-range is the best choice for finding the value in sorted vector
		// ("Effective STL", Scott Meyers, item 45)
		auto it = std::equal_range(pRows.cbegin(), pRows.cend(), lookedAt,
			[&] (const TupleString& tuple1, const TupleString& tuple2) -> bool
			{
				return (std::get<indexKeyword>(tuple1) == std::get<indexKeyword>(tuple2)
					&& std::get<indexValue>(tuple1) == std::get<indexValue>(tuple2));
			}
		);

		if (it.first == it.second)
			return false;

		photographer = std::get<GenericTools::IndexOf<Owner, Tuple>::value>(*(it.first));

		return true;
	}


	void Cameras::addNewCamera(const Keyword::StringType& currentKeyword,
		const Value::StringType& value,
		const Owner::StringType& photographer)
	{
		// Determine names of the fields in the database
		std::vector<YString> fieldNames;
		Private::TupleFields<Tuple>::FieldNames(fieldNames);

		// Create a tuple with new elements to introduce
		TupleString newTuple;
		std::get<GenericTools::IndexOf<Owner, Tuple>::value>(newTuple) = photographer;
		std::get<GenericTools::IndexOf<Value, Tuple>::value>(newTuple) = value;
		std::get<GenericTools::IndexOf<Keyword, Tuple>::value>(newTuple) = currentKeyword;

		// Add the camera in the database (which will check by the way if the insertion is legit or not)
		pDatabase.insertData(TableName(), fieldNames, newTuple);

		// If database accepted the query without throwing exception, all is ok
		// Add the camera in the memory
		addNewTuple(newTuple);

		// Do not forget to reorder pRows!
		// As insertion will be quite a rare event, it is sensical to reorder it whenever it happens
		// instead of using a set or alike
		enum { indexKeyword = GenericTools::IndexOf<Keyword, Tuple>::value };

		std::sort(pRows.begin(), pRows.end(), [](const TupleString& tuple1, const TupleString& tuple2) -> bool
			{
				return (std::get<indexKeyword>(tuple1) < std::get<indexKeyword>(tuple2));
			});

	}

} // namespace ExtendedPhoto
} // namespace PictStock
