#ifndef __PICT_STOCK__DATABASE__PHOTOGRAPHERS_HPP_
# define __PICT_STOCK__DATABASE__PHOTOGRAPHERS_HPP_

# include <tuple>
# include <vector>
# include <unordered_set>
# include <yuni/core/string.h>
# include "../pict_stock.hpp"
# include "../tools/tools.hpp"
# include "../tools/tuple_utilities.hpp"
# include "photographer.hpp"
# include "private/table_photographers.hpp"
# include "private/table.hpp"

namespace GenericTools
{
	class SqliteWrapper;
}



namespace PictStock
{
namespace Database
{
    namespace Schema
    {
        struct Photographers
        {
            static Yuni::CString<15, false> Name() { return "Photographers"; }
            static YString Schema()
            {
                return
                    "FirstName varchar(80),"
                    "LastName varchar(80),"
                    "Abbreviation varchar(8) PRIMARY KEY NOT NULL,"
                    "UNIQUE (FirstName, LastName) ON CONFLICT ABORT";
            }
        };
    } // namespace Schema




	/*!
	** \brief This class handles all the known photographers, including
	** their storing inside sqlite database
	*/
    class YUNI_DECL Photographers : private Yuni::NonCopyable<Photographers>,
                                    public Private::Table<Schema::Photographers>
	{

	public:

		//! Tuple type for rows of the table cameras in sqlite database
		typedef TablePhotographers::Tuple Tuple;

		/*!
		** \brief Yields the WrappedType hold by each element of the input tuple
		**
		** For instance, if TupleT = [Keyword, Value, Owner] keyword type will yield
		** 	[Keyword::WrappedType, Value::WrappedType, Owner::WrappedType]
		*/
		typedef TablePhotographers::TupleWrappedType TupleWrappedType;

		//! Name of the table in sqlite database
		static YString TableName() { return "Photographers"; }

	public:

		//! Constructor(s)
		//@{
		/*!
		 * \brief Constructor
		 */
        explicit Photographers(GenericTools::SqliteWrapper& database,
            nsTable::Values mode = nsTable::load);
		//@}

		//! Add a new photographer
		void addNewPhotographer(const TablePhotographers::FirstName::WrappedType& firstName,
			const TablePhotographers::LastName::WrappedType& lastName,
			const TablePhotographers::Abbreviation::WrappedType& abbreviation);

		/*!
		** \brief Find the photographer whose abbreviation is the one given in input
		*/
		bool findPhotographer(Photographer::Ptr& photographer,
			const TablePhotographers::Abbreviation::WrappedType& abbreviation) const;


    private:

        /*!
        ** \brief Load existing data
        **
        ** Expected to be called in constructor if load mode
        */
        void load();


	private:

		//! Reference to database object
		GenericTools::SqliteWrapper& pDatabase;

		/*!
		** \brief Data
		**
		** Each entry matches a row in the sqlite database
		**
		*/
		Photographer::Vector pData;

	};

} // namespace Database
} // namespace PictStock

# include "photographers.hxx"

#endif /* __PICT_STOCK__DATABASE__PHOTOGRAPHERS_HPP_ */
