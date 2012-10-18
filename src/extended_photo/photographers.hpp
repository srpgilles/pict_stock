#ifndef PHOTOGRAPHERS_HPP_
# define PHOTOGRAPHERS_HPP_

# include <tuple>
# include <vector>
# include <unordered_set>
# include <yuni/core/string.h>
# include "../pict_stock.hpp"
# include "../tools/tools.hpp"
# include "private/helpers.hpp"
# include "private/photographer.hpp"
# include "private/table_photographers.hpp"

namespace GenericTools
{
	class SqliteWrapper;
}



namespace PictStock
{
namespace ExtendedPhoto
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
		** \brief Yields the StringType hold by each element of the input tuple
		**
		** For instance, if TupleT = [Keyword, Value, Owner] keyword type will yield
		** 	[Keyword::StringType, Value::StringType, Owner::StringType]
		*/
		typedef TablePhotographers::TupleString TupleString;

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

		//! Add a new camera
		void addNewPhotographer(const TablePhotographers::FirstName::StringType& firstName,
			const TablePhotographers::LastName::StringType& lastName,
			const TablePhotographers::Abbreviation::StringType& abbreviation);


	private:

		//! Reference to database object
		GenericTools::SqliteWrapper& pDatabase;

		/*!
		** \brief Data
		**
		** Each entry matches a row in the sqlite database
		**
		*/
		Private::Photographer::Vector pData;

	};

} // namespace ExtendedPhoto
} // namespace PictStock

# include "photographers.hxx"

#endif /* PHOTOGRAPHERS_HPP_ */
