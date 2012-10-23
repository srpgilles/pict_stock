#ifndef CAMERA_HELPERS_HPP_
# define CAMERA_HELPERS_HPP_

# include <deque>

namespace PictStock
{
namespace ExtendedPhoto
{
namespace Private
{

/*!
	** \brief Helper class when a tuple has been used to "typify" more deeply a type
	**
	** An example will explain much better I think: when reading database data,
	** three columns are considered. All these three columns are strings, potentially
	** of the same type
	**
	** I want to be able to be able to distinguish between those data, so a wrapper class is
	** added. So for instance if columns are
	** 	"Foo" [ type std::string ], 
	** 	"Bar" [ type YString ]
	** 	"Baz" [ type YString ],
	** I create a struct for each of them (conveniently named Foo, Bar, Baz)
	**
	** Then I define a tuple (which acts TupleT below) as:
	** 		TupleT = { Foo, Bar, Baz }
	**
	** So I can statically look for which index matches Bar by
	** 		GenericTools::IndexOf<Bar, TupleT>::value
	**
	** The initial types are wrapped in those tuples; so for instance Foo::WrappedType yields std::string
	**
	** TupleWrappedType structure yields the wrapped type as a tuple.
	**  So TupleWrappedType<TupleT>::WrappedType is a tuple { std::string, YString, YString } in the example.
	**
	** TupleFields::FieldNames(std::vector<YString>& fields) enables to obtain the names of the column
	** ({"Foo", "Bar", "Baz"} in our example
	**
	*/


	template<unsigned int Index, unsigned int Max, typename TupleT>
	struct Helper
	{
		typedef typename std::tuple_element<Index, TupleT>::type ElementType;
		typedef typename ElementType::WrappedType WrappedTupleType;

		typedef decltype(
			std::tuple_cat(
				typename Helper<Index - 1, Max, TupleT>::type(),
				std::make_tuple(WrappedTupleType())
				)
						) type;

		static void FieldNames(std::deque<YString>& fields)
		{
			fields.push_front(ElementType::FieldName());
			Helper<Index - 1, Max, TupleT>::FieldNames(fields);
		}
	};


	template<unsigned int Max, typename TupleT>
	struct Helper<0u, Max, TupleT>
	{
		typedef typename std::tuple_element<0u, TupleT>::type ElementType;
		typedef typename ElementType::WrappedType WrappedTupleType;
		typedef decltype(std::make_tuple(WrappedTupleType())) type;

		static void FieldNames(std::deque<YString>& fields)
		{
			fields.push_front(ElementType::FieldName());
		}
	};

	/*!
	** \brief This structure yields the StringType hold by each element of the input tuple
	**
	** For instance, if TupleT = [Keyword, Value, Owner] keyword "type" will yield
	** 	[Keyword::WrappedType, Value::Keyword::WrappedType, Owner::Keyword::WrappedType]
	**
	** 	See #Helper for much more details
	*/

	template<typename TupleT>
	struct TupleWrappedType
	{
		typedef typename Helper<std::tuple_size<TupleT>::value - 1u, std::tuple_size<TupleT>::value, TupleT>::type type;
	};


	/*!
	** \brief This structure yields the names of the fields of the input tuple
	**
	** For instance, if TupleT = [Keyword, Value, Owner], keyword "value" will yield
	** 	std::deque([Keyword::fieldName(), Value::fieldName(), Owner::fieldName()])
	**
	** 	See #Helper for much more details
	*/
	template<typename TupleT>
	struct TupleFields
	{
		static void FieldNames(std::vector<YString>& fields)
		{
			enum { size = std::tuple_size<TupleT>::value };

			fields.resize(size);

			std::deque<YString> buf;
			Helper<size - 1u, size, TupleT>::FieldNames(buf);

			assert(buf.size() == size);

			std::move(buf.begin(), buf.end(), fields.begin());
		}
	};


} // namespace Private
} // namespace ExtendedPhoto
} // namespace PictStock




#endif /* CAMERA_HELPERS_HPP_ */
