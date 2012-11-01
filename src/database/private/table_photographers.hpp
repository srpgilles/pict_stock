#ifndef __PICT_STOCK__DATABASE__PRIVATE__TABLE_PHOTOGRAPHERS_HPP_
# define __PICT_STOCK__DATABASE__PRIVATE__TABLE_PHOTOGRAPHERS_HPP_

# include <tuple>
# include <yuni/core/string.h>
# include "../../tools/tuple_utilities.hpp"

namespace PictStock
{
namespace Database
{
namespace TablePhotographers
{
	struct FirstName
	{
		typedef Yuni::CString<80, false> WrappedType;
		static YString FieldName() { return "FirstName"; }
	};

	struct LastName
	{
		typedef Yuni::CString<80, false> WrappedType;
		static YString FieldName() { return "LastName"; }
	};

	struct Abbreviation
	{
		typedef Yuni::CString<8, false> WrappedType;
		static YString FieldName() { return "Abbreviation"; }
	};

	//! Tuple type for rows of the table cameras in sqlite database
	typedef std::tuple<FirstName, LastName, Abbreviation> Tuple;

	/*!
	** \brief Yields the StringType hold by each element of the input tuple
	**
	** For instance, if TupleT = [Keyword, Value, Owner] keyword type will yield
	** 	[Keyword::StringType, Value::StringType, Owner::StringType]
	*/
    typedef GenericTools::Tuple::WrappedType<Tuple>::type TupleWrappedType;

} // namespace TablePhotographers
} // namespace Database
} // namespace PictStock


#endif /* __PICT_STOCK__DATABASE__PRIVATE__TABLE_PHOTOGRAPHERS_HPP_ */
