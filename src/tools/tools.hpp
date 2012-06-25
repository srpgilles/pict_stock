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


} // namespace GenericTools
