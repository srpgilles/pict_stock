#include "cameras.hpp"
#include "../tools/sqlite_wrapper.hpp"
#include "../tools/tools.hpp"


namespace PictStock
{
namespace ExtendedPhoto
{

	using namespace TableCameras;

	Cameras::Cameras(GenericTools::SqliteWrapper& database)
		: pDatabase(database)
	{
		database.select(pRows, "Keyword,Value,Owner FROM Cameras ORDER BY Keyword");
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
		pDatabase.insertData("Cameras", fieldNames, newTuple);

		// If database accepted the query without trhowing exception, all is ok
		// Add the camera in the memory
		pRows.push_back(newTuple);

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
