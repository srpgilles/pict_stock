#ifndef __PICT_STOCK__TOOLS__TUPLE_UTILITIES_HPP_
# define __PICT_STOCK__TOOLS__TUPLE_UTILITIES_HPP_

# include <deque>
# include <tuple>
# include <type_traits>
# include <yuni/core/string.h>

# include "tools.hpp"

namespace GenericTools
{
namespace Tuple
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
    ** 		GenericTools::Tuple::IndexOf<Bar, TupleT>::value
	**
	** The initial types are wrapped in those tuples; so for instance Foo::WrappedType yields std::string
	**
	** TupleWrappedType structure yields the wrapped type as a tuple.
	**  So TupleWrappedType<TupleT>::WrappedType is a tuple { std::string, YString, YString } in the example.
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
    struct WrappedType
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
    struct Fields
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


    /*!
    ** \brief helper class for IndexOf, which does much of the actual work
    **
    ** This is called recursively in IndexOf until the class is found or the
    ** list has been completely checked
    **
    ** \tparam T Class looked at in the tuple
    ** \tparam TupleT std::tuple which is investigated
    ** \tparam I Index of the current element index in the typelist
    */
    template<class T, class TupleT, std::size_t I>
    struct IndexOfHelper
    {
    private:

        typedef typename std::tuple_element<I, TupleT>::type type;
        enum { found = std::is_same<T, type>::value };

    public:

        // Static_cast to avoid gcc warning about enumeral/non enumeral
        enum { value = (found == 1 ? static_cast<int>(I)
            : static_cast<int>(IndexOfHelper<T, TupleT, I-1>::value)) };
    };


    template<class T, class TupleT>
    struct IndexOfHelper<T, TupleT, 0>
    {
        typedef typename std::tuple_element<0, TupleT>::type type;
        enum { value = (std::is_same<T, type>::value == 1 ? 0 : -1) };
    };


    /*!
    ** \brief Find the index of a given type in a tuple (used here as a typelist)
    **
    ** If none found, value is -1, otherwise the index is returned
    **
    **
    ** If a same structure appears several time in the tuple (which doesn't make
    ** much sense for a typelist), the index of the last occurrence is returned
    ** (not the most logical but I had to workaround with C++ standard limitations)
    **
    ** \tparam T Class looked at in the tuple
    ** \tparam TupleT std::tuple which is investigated
    */
    template<class T, class TupleT>
    struct IndexOf
    {
    private:
        enum { lastIndex = std::tuple_size<TupleT>::value - 1 };

    public:
        enum { value = IndexOfHelper<T, TupleT, lastIndex>::value };
    };


    /*!
    ** \brief Facility to print elements of a tuple
    **
    ** Inspired by Nicolai M. Josuttis "The C++ standard library" page 74
    */
    template<class StreamT, unsigned int Index, unsigned int Max, typename... Args>
    struct printHelper
    {
        template<typename StringT>
        static void print(StreamT& stream, const std::tuple<Args...>&t, const StringT& separator = ",")
        {

            typedef std::tuple<Args...> TupleType;

            typedef typename std::tuple_element<Index, TupleType>::type EltTupleType;

            Yuni::CString<4, false> quote =
                Yuni::Traits::IsString<EltTupleType>::yes ? "\"" : "";

            stream << quote << std::get<Index>(t) << quote << (Index + 1 == Max ? "" : separator);
            printHelper<StreamT, Index + 1, Max, Args...>::print(stream, t, separator);
        }
    };


    template<class StreamT, unsigned int Max, typename... Args>
    struct printHelper<StreamT, Max, Max, Args...>
    {
        template<typename StringT>
        static void print(StreamT&, const std::tuple<Args...>&, const StringT&)
        { }
    };


    /*!
    ** \brief Print the content of a tuple
    */
    template<typename StreamT, typename StringT = YString, typename... Args>
    void print(StreamT& stream, const std::tuple<Args...>&t, StringT separator = ",",
        StringT opener = "[", StringT closer = "]\n")
    {
        stream << opener;
        printHelper<StreamT, 0, sizeof...(Args), Args...>::print(stream, t, separator);
        stream << closer;
    }

} // namespace Tuple
} // namespace GenericTools


#endif /* __PICT_STOCK__TOOLS__TUPLE_UTILITIES_HPP_ */
