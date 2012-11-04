#ifndef __PICT_STOCK__TOOLS__TOOLS_HPP_
# define __PICT_STOCK__TOOLS__TOOLS_HPP_

# include <ostream>
# include <type_traits>
# include <yuni/core/string.h>


namespace GenericTools
{




    /*template<class T>
	struct IsString
	{
	private:

		typedef typename std::remove_const<T>::type WithoutConst;
		typedef typename std::remove_pointer<WithoutConst>::type WithoutConstAndPointers;
		enum { isCharRelated = std::is_same<WithoutConstAndPointers, char>::value };


	public:

		enum { value = isCharRelated ? 1 : std::is_constructible<T, char*>::value };
    }*/



    /*!
    ** \brief Print the content of a container
    */
    template<typename StreamT, class ContainerT, typename StringT>
    void printContainer(StreamT& stream, const ContainerT& container,
        StringT separator = ",", StringT opener = "[", StringT closer = "]\n")
    {
        stream << opener;

        auto it = container.cbegin();
        auto end = container.cend();    
        auto size = container.size();
        (void) end; // avoid compilation warning in release mode

        for (decltype(size) i = 0u; i < size - 1u; ++it, ++i)
        {            
            assert(it != end);
            stream << *it << separator;
        }

        if (size > 0u)
            stream << *it;

        stream << closer;
    }




} // namespace GenericTools

#endif /* __PICT_STOCK__TOOLS__TOOLS_HPP_ */
