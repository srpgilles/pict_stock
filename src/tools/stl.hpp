#ifndef __PICT_STOCK__TOOLS__STL_HPP_
# define __PICT_STOCK__TOOLS__STL_HPP_

# include <functional>
# include <string>
# include <yuni/core/string.h>

namespace std
{
    //! Provide hash function for Yuni String
    template<>
    class hash<YString>
    {
    public:
        size_t operator()(const YString& str) const
        {
            //return std::hash<std::string>()(str.to<std::string>());
            return str.hash();
        }
    };
}



#endif /* __PICT_STOCK__TOOLS__STL_HPP_ */
