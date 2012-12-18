#ifndef __PICT_STOCK__TOOLS__TUPLE_UTILITIES_MSVC__TUPLE_UTILITIES_N2_HPP_
# define __PICT_STOCK__TOOLS__TUPLE_UTILITIES_MSVC__TUPLE_UTILITIES_N2_HPP_


    /*!
    ** \brief Facility to print elements of a tuple
    **
    ** Inspired by Nicolai M. Josuttis "The C++ standard library" page 74
    */
    template<class StreamT, unsigned int Index, unsigned int Max, typename U1, typename U2>
    struct printHelper2
    {
        template<typename StringT>
        static void print(StreamT& stream, const std::tuple<U1, U2>&t, const StringT& separator = ",")
        {
            typedef std::tuple<U1, U2> TupleType;

            typedef typename std::tuple_element<Index, TupleType>::type EltTupleType;

            Yuni::CString<4, false> quote =
                Yuni::Traits::IsString<EltTupleType>::yes ? "\"" : "";

            stream << quote << std::get<Index>(t) << quote << (Index + 1 == Max ? "" : separator);
            printHelper2<StreamT, Index + 1, Max, U1, U2>::print(stream, t, separator);
        }
    };



    template<class StreamT, unsigned int Max, typename U1, typename U2>
    struct printHelper2<StreamT, Max, Max, U1, U2>
    {
        template<typename StringT>
        static void print(StreamT&, const std::tuple<U1, U2>&, const StringT&)
        { }
    };



    /*!
    ** \brief Print the content of a tuple
    */
    template<typename StreamT, typename StringT, typename U1, typename U2>
    void print(StreamT& stream, const std::tuple<U1, U2>&t, StringT separator = ",",
        StringT opener = "[", StringT closer = "]\n")
    {
        stream << opener;
        printHelper2<StreamT, 0, 1, U1, U2>::print(stream, t, separator);
        stream << closer;
    }



#endif /* __PICT_STOCK__TOOLS__TUPLE_UTILITIES_MSVC__TUPLE_UTILITIES_N2_HPP_ */
