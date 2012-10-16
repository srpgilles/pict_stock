#ifndef CAMERA_HELPERS_HPP_
# define CAMERA_HELPERS_HPP_

namespace PictStock
{
namespace NSCameras
{
namespace Private
{
	struct Keyword
	{
		typedef std::string StringType;
	};

	struct Value
	{
		typedef Yuni::CString<80, false> StringType;
	};

	struct Owner
	{
		typedef Yuni::CString<8, false> StringType;
	};

	template<unsigned int Index, unsigned int Max, typename TupleT>
	struct TupleStringHelper
	{
		typedef typename std::tuple_element<Index, TupleT>::type::StringType EltTupleType;

		typedef decltype(
			std::tuple_cat(
				typename TupleStringHelper<Index - 1, Max, TupleT>::type(),
				std::make_tuple(EltTupleType())
				)
						) type;

	};


	template<unsigned int Max, typename TupleT>
	struct TupleStringHelper<0u, Max, TupleT>
	{
		typedef typename std::tuple_element<0u, TupleT>::type::StringType EltTupleType;
		typedef decltype(std::make_tuple(EltTupleType())) type;
	};

	/*!
	** \brief This structure yields the StringType hold by each element of the input tuple
	**
	** For instance, if TupleT = [Keyword, Value, Owner] keyword type will yield
	** 	[Keyword::StringType, Value::StringType, Owner::StringType]
	*/

	template<typename TupleT>
	struct TupleString
	{
		typedef typename TupleStringHelper<std::tuple_size<TupleT>::value - 1u, std::tuple_size<TupleT>::value, TupleT>::type type;
	};


} // namespace Private
} // namespace NSCameras
} // namespace PictStock




#endif /* CAMERA_HELPERS_HPP_ */
