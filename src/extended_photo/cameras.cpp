#include "cameras.hpp"
#include "../tools/sqlite_wrapper.hpp"
#include "../tools/tools.hpp"


namespace PictStock
{
	using namespace NSCameras::Private;

	Cameras::Cameras(GenericTools::SqliteWrapper& database)
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


} // namespace PictStock
