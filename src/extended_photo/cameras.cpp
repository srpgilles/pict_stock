#include "cameras.hpp"
#include "photographers.hpp"
#include "../tools/sqlite_wrapper.hpp"
#include "../tools/exceptions.hpp"
#include <ostream>

#ifndef NDEBUG
# include "../tools/tools.hpp"
#endif // NDEBUG


namespace PictStock
{
namespace ExtendedPhoto
{

	using namespace TableCameras;

	Cameras::Cameras(GenericTools::SqliteWrapper& database)
		: pDatabase(database),
		  pPhotographersPtr(nullptr)
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
				command << " FROM " << TableName() << " ORDER BY " << Owner::FieldName() << ';';
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

		{
			// Init photographers
			std::unique_ptr<Photographers> buf(new Photographers(database));
			pPhotographersPtr = std::move(buf);
		}
	}

	bool Cameras::identifyPhotographer(
		const Keyword::StringType& currentKeyword,
		const Value::StringType& valueToCheck,
		Photographer::Ptr& photographer) const
	{
		enum { indexKeyword = GenericTools::IndexOf<Keyword, Tuple>::value };
		enum { indexValue = GenericTools::IndexOf<Value, Tuple>::value };

		auto end = pRows.cend();
		auto it = std::find_if(pRows.cbegin(), end,
			[&] (const TupleString& tuple) -> bool
			{
				return (std::get<indexKeyword>(tuple) == currentKeyword
					&& std::get<indexValue>(tuple) == valueToCheck);
			}
		);

		if (it == end)
			return false;

		assert(it != pRows.end());

		// We get the abbreviation; let's fetch the complete photographer object
		assert(!(!pPhotographersPtr));

		GenericTools::printTuple(std::cout, *it);
		auto abbreviation =	std::get<GenericTools::IndexOf<Owner, Tuple>::value>(*it);
		std::cout << "CAMERA ABBREV = " << abbreviation << '\n';
		identifyPhotographerAbbr(abbreviation, photographer);

		return true;
	}



	void Cameras::identifyPhotographerAbbr(
		const TableCameras::Owner::StringType& abbreviation,
		Photographer::Ptr& photographer) const
	{
		if (!pPhotographersPtr->findPhotographer(photographer, abbreviation))
		{
			YString message("Malformed database (or bug in the code): a camera was associated "
				"to photographer whose abbreviation is \"");
			message << abbreviation << "\" but none was found in photographer table.";
			throw GenericTools::Exception(message);
		}

		assert(!(!photographer));
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


	#ifndef NDEBUG
	void Cameras::printRows() const
	{
		std::cout << "All the rows (" << pRows.size() << ") found in memory are:\n";
		std::for_each(pRows.cbegin(), pRows.cend(), [](const TupleString& tuple)
			{
				GenericTools::printTuple(std::cout, tuple, ",", "\t[", "]\n");
			}
		);
		std::cout << "-------------------------------\n";
	}
	#endif // NDEBUG

} // namespace ExtendedPhoto
} // namespace PictStock
