#ifndef TOOLS_HPP
# define TOOLS_HPP

# include <ostream>
# include <type_traits>
# include <yuni/core/string.h>


namespace GenericTools
{

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
    ** \brief Print the content of a container
    */
    template<typename StreamT, class ContainerT, typename StringT = YString>
    void printContainer(StreamT& stream, const ContainerT& container,
        StringT separator = ",", StringT opener = "[", StringT closer = "]\n")
    {
        stream << opener;

        auto it = container.cbegin();
        auto end = container.cend();
        auto lastElement = end;
        --lastElement;

        for (; it != lastElement; ++it)
            stream << *it << separator;

        if (lastElement != end)
            stream << *lastElement;

        stream << closer;
    }




} // namespace GenericTools

#endif // TOOLS_HPP
