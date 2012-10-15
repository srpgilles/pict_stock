#include <ostream>
#include <tuple>
#include <yuni/core/string.h>
#include <type_traits>

#ifndef TOOLS_HPP
# define TOOLS_HPP

namespace GenericTools
{


	/*!
	** \brief A structure to tell whether two classes are the same or not
	*/
	template<class T, class U>
	struct IsSame
	{
		enum { value = 0 };
	};

	template<class T>
	struct IsSame<T, T>
	{
		enum { value = 1 };
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
		enum { found = IsSame<T, type>::value };

	public:

		// Static_cast to avoid gcc warning about enumeral/non enumeral
		enum { value = (found == 1 ? static_cast<int>(I)
			: static_cast<int>(IndexOfHelper<T, TupleT, I-1>::value)) };
	};


	template<class T, class TupleT>
	struct IndexOfHelper<T, TupleT, 0>
	{
		typedef typename std::tuple_element<0, TupleT>::type type;
		enum { value = (IsSame<T, type>::value == 1 ? 0 : -1) };
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

	template<class T>
	struct IsString
	{
	private:

		typedef typename std::remove_const<T>::type WithoutConst;
		typedef typename std::remove_pointer<WithoutConst>::type WithoutConstAndPointers;
		enum { isCharRelated = std::is_same<WithoutConstAndPointers, char>::value };


	public:

		enum { value = isCharRelated ? 1 : std::is_constructible<T, char*>::value };
	};




	/*!
	** \brief Facility to print elements of a tuple
	**
	** Inspired by Nicolai M. Josuttis "The C++ standard library" page 74
	*/
	template<class StreamT, unsigned int Index, unsigned int Max, typename... Args>
	struct printTupleHelper
	{
		template<typename StringT>
		static void print(StreamT& stream, const std::tuple<Args...>&t, const StringT& separator = ',')
		{

			typedef std::tuple<Args...> TupleType;

			typedef typename std::tuple_element<Index, TupleType>::type EltTupleType;

			Yuni::CString<4, false> quote = IsString<EltTupleType>::value ? "\"" : "";

			stream << quote << std::get<Index>(t) << quote << (Index + 1 == Max ? "" : separator);
			printTupleHelper<StreamT, Index + 1, Max, Args...>::print(stream, t, separator);
		}
	};


	template<class StreamT, unsigned int Max, typename... Args>
	struct printTupleHelper<StreamT, Max, Max, Args...>
	{
		template<typename StringT>
		static void print(StreamT&, const std::tuple<Args...>&, const StringT&)
		{ }
	};


	template<typename StreamT, typename StringT, typename... Args>
	void printTuple (StreamT& stream, const std::tuple<Args...>&t, StringT separator = ',',
		StringT opener = '[', StringT closer = ']')
	{
		stream << opener;
		printTupleHelper<StreamT, 0, sizeof...(Args), Args...>::print(stream, t, separator);
		stream << closer;
	}


	//! Helper to determine whether a type is meant to dewcribe strings
	typedef std::tuple<char, char*, std::string, YString> StringTuple;




} // namespace GenericTools

#endif // TOOLS_HPP
