#ifndef TOOLS_STL_HPP
# define TOOLS_STL_HPP

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
            return std::hash<std::string>()(str.to<std::string>());
        }
    };
}



#endif // STL_HPP
