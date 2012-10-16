#ifndef CAMERAS_HPP_
# define CAMERAS_HPP_

# include <tuple>
# include <vector>
# include <yuni/core/string.h>
# include "../pict_stock.hpp"
# include "private/photographer.hpp"


namespace GenericTools
{
	class SqliteWrapper;
}

namespace PictStock
{
	namespace NSCameras
	{
	namespace Private
	{
		struct Keyword
		{
			typedef Yuni::CString<80, false> StringType;
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


	/*!
	** \brief This class handles all the known photographers and cameras, including
	** their storing inside sqlite database
	*/
	class YUNI_DECL Cameras
	{

	public:

		//! Tuple type for rows of the table cameras in sqlite database
		typedef std::tuple<NSCameras::Private::Keyword, NSCameras::Private::Value,
			NSCameras::Private::Owner> Tuple;

		/*!
		** \brief Yields the StringType hold by each element of the input tuple
		**
		** For instance, if TupleT = [Keyword, Value, Owner] keyword type will yield
		** 	[Keyword::StringType, Value::StringType, Owner::StringType]
		*/
		typedef NSCameras::Private::TupleString<Tuple>::type  TupleString;

	public:

		//! Constructor(s)
		//@{
		/*!
		 * \brief Constructor
		 */
		explicit Cameras(GenericTools::SqliteWrapper& database);


		//@}

	private:

		//! Rows in sqlite database table related to cameras
		std::vector<TupleString> pRows;


	};


} // namespace PictStock

#endif /* PHOTOGRAPHERS_HPP_ */
