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
namespace Database
{
    namespace
    {
        static YString schema()
        {
            YString ret("Keyword varchar(80),"
                "Value varchar(80),"
                "Owner varchar(8),"
                "FOREIGN KEY(Owner) REFERENCES Photographers(Abbreviation),"
                "UNIQUE (Keyword, Value) ON CONFLICT ABORT");

            return std::move(ret);
        }

    } // namespace anonymous


	using namespace TableCameras;

    Cameras::Cameras(GenericTools::SqliteWrapper& database,
        const Photographers& photographers, nsTable::Values mode)
        : Private::Table("Cameras", schema()),
          pDatabase(database),
          pPhotographers(photographers)
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

    void Cameras::load()
    {
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
                command << " FROM " << tableName() << " ORDER BY " << Owner::FieldName() << ';';
            }

            std::cout << "COMMAND = " << command << '\n';
            pDatabase.select(pRows, command);
            std::cout << "OK\n";
        }

        {
            // Also init #pKeywords
            typedef std::tuple<Keyword::WrappedType> TupleKeyword;
            std::vector<TupleKeyword> keywords;
            YString command("Keyword FROM ");
            command << tableName();
            pDatabase.select(keywords, command);

            for (auto it = keywords.begin(), end = keywords.end(); it != end; ++it)
                pKeywords.insert(std::get<0>(*it));
        }
	}

	bool Cameras::identifyPhotographer(
		const Keyword::WrappedType& currentKeyword,
		const Value::WrappedType& valueToCheck,
		Photographer::Ptr& photographer) const
	{
        enum { indexKeyword = GenericTools::Tuple::IndexOf<Keyword, Tuple>::value };
        enum { indexValue = GenericTools::Tuple::IndexOf<Value, Tuple>::value };

		auto end = pRows.cend();
		auto it = std::find_if(pRows.cbegin(), end,
			[&] (const TupleWrappedType& tuple) -> bool
			{
				return (std::get<indexKeyword>(tuple) == currentKeyword
					&& std::get<indexValue>(tuple) == valueToCheck);
			}
		);

		if (it == end)
			return false;

		// We get the abbreviation; let's fetch the complete photographer object
        auto abbreviation =	std::get<GenericTools::Tuple::IndexOf<Owner, Tuple>::value>(*it);
		identifyPhotographerAbbr(abbreviation, photographer);

		return true;
	}



	void Cameras::identifyPhotographerAbbr(
		const TableCameras::Owner::WrappedType& abbreviation,
		Photographer::Ptr& photographer) const
	{
        if (!pPhotographers.findPhotographer(photographer, abbreviation))
		{
			YString message("Malformed database (or bug in the code): a camera was associated "
				"to photographer whose abbreviation is \"");
			message << abbreviation << "\" but none was found in photographer table.";
			throw GenericTools::Exception(message);
		}

		assert(!(!photographer));
	}


	void Cameras::addNewCamera(const Keyword::WrappedType& currentKeyword,
		const Value::WrappedType& value,
		const Owner::WrappedType& photographer)
	{
		// Determine names of the fields in the database
		std::vector<YString> fieldNames;
        GenericTools::Tuple::Fields<Tuple>::FieldNames(fieldNames);

		// Create a tuple with new elements to introduce
		TupleWrappedType newTuple;
        std::get<GenericTools::Tuple::IndexOf<Owner, Tuple>::value>(newTuple) = photographer;
        std::get<GenericTools::Tuple::IndexOf<Value, Tuple>::value>(newTuple) = value;
        std::get<GenericTools::Tuple::IndexOf<Keyword, Tuple>::value>(newTuple) = currentKeyword;

		// Add the camera in the database (which will check by the way if the insertion is legit or not)
        pDatabase.insertData(tableName(), fieldNames, newTuple);

		// If database accepted the query without throwing exception, all is ok
		// Add the camera in the memory
		addNewTuple(newTuple);

		// Do not forget to reorder pRows!
		// As insertion will be quite a rare event, it is sensical to reorder it whenever it happens
		// instead of using a set or alike
        enum { indexKeyword = GenericTools::Tuple::IndexOf<Keyword, Tuple>::value };

		std::sort(pRows.begin(), pRows.end(), [](const TupleWrappedType& tuple1, const TupleWrappedType& tuple2) -> bool
			{
				return (std::get<indexKeyword>(tuple1) < std::get<indexKeyword>(tuple2));
			});

	}

    #ifndef NDEBUG
	void Cameras::printRows() const
	{
		std::cout << "All the rows (" << pRows.size() << ") found in memory are:\n";
		std::for_each(pRows.cbegin(), pRows.cend(), [](const TupleWrappedType& tuple)
			{
                GenericTools::Tuple::print(std::cout, tuple, ",", "\t[", "]\n");
			}
		);
		std::cout << "-------------------------------\n";
	}
	#endif // NDEBUG

} // namespace Database
} // namespace PictStock
