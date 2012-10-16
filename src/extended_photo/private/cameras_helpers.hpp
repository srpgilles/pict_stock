#ifndef CAMERA_HELPERS_HPP_
# define CAMERA_HELPERS_HPP_

# include <deque>

namespace PictStock
{
namespace NSCameras
{
namespace Private
{
	struct Keyword
	{
		typedef std::string StringType;
		static YString FieldName() { return "Keyword"; }
	};

	struct Value
	{
		typedef Yuni::CString<80, false> StringType;
		static YString FieldName() { return "Value"; }
	};

	struct Owner
	{
		typedef Yuni::CString<8, false> StringType;
		static YString FieldName() { return "Owner"; }
	};

	template<unsigned int Index, unsigned int Max, typename TupleT>
	struct Helper
	{
		typedef typename std::tuple_element<Index, TupleT>::type ElementType;
		typedef typename ElementType::StringType StringTupleType;

		typedef decltype(
			std::tuple_cat(
				typename Helper<Index - 1, Max, TupleT>::type(),
				std::make_tuple(StringTupleType())
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
		typedef typename ElementType::StringType StringTupleType;
		typedef decltype(std::make_tuple(StringTupleType())) type;

		static void FieldNames(std::deque<YString>& fields)
		{
			fields.push_front(ElementType::FieldName());
		}
	};

	/*!
	** \brief This structure yields the StringType hold by each element of the input tuple
	**
	** For instance, if TupleT = [Keyword, Value, Owner] keyword "type" will yield
	** 	[Keyword::StringType, Value::StringType, Owner::StringType]
	*/

	template<typename TupleT>
	struct TupleString
	{
		typedef typename Helper<std::tuple_size<TupleT>::value - 1u, std::tuple_size<TupleT>::value, TupleT>::type type;
	};


	/*!
	** \brief This structure yields the names of the field  of the input tuple
	**
	** For instance, if TupleT = [Keyword, Value, Owner], keyword "value" will yield
	** 	std::deque([Keyword::fieldName(), Value::fieldName(), Owner::fieldName()])
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

			std::copy(buf.begin(), buf.end(), fields.begin());
		}
	};







	/*!
	 *
	 */
	/*template<unsigned int Index, typename TupleT>
	struct FieldNamesHelper
	{
		static void FieldNames()
		{

		}
	};*/


} // namespace Private
} // namespace NSCameras
} // namespace PictStock




#endif /* CAMERA_HELPERS_HPP_ */
