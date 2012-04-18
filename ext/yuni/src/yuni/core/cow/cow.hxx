#ifndef __YUNI_CORE_COW_COW_HXX__
# define __YUNI_CORE_COW_COW_HXX__


namespace Yuni
{


	template<typename T, template <class> class OwspP, template <class> class ChckP,
		class ConvP,
		template <class> class StorP, template <class> class ConsP>
	inline void
	COW<T,OwspP,ChckP,ConvP,StorP,ConsP>::copy()
	{
		// Copy nothing if unique
		if (!pValue.unique())
		{
			T* pointer = SmartPtrType::WeakPointer(pValue);
			if (!pointer)
				pValue = new T();
			else
				pValue = new T(*pointer);
		}
	}





} // namespace Yuni

#endif // __YUNI_CORE_COW_COW_HXX__
