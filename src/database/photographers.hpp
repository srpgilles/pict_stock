#ifndef PHOTOGRAPHERS_HPP_
# define PHOTOGRAPHERS_HPP_

# include <tuple>
# include <vector>
# include <unordered_set>
# include <yuni/core/string.h>
# include "../pict_stock.hpp"
# include "../tools/tools.hpp"
# include "private/helpers.hpp"
# include "photographer.hpp"
# include "private/table_photographers.hpp"

namespace GenericTools
{
	class SqliteWrapper;
}



namespace PictStock
{
namespace Database
{


	/*!
	** \brief This class handles all the known photographers, including
	** their storing inside sqlite database
	*/
	class YUNI_DECL Photographers : private Yuni::NonCopyable<Photographers>
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
		explicit Photographers(GenericTools::SqliteWrapper& database);
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

#endif /* PHOTOGRAPHERS_HPP_ */
