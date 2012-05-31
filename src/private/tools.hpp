#ifndef _PRIVATE_TOOLS_HPP_
# define _PRIVATE_TOOLS_HPP_

# include "relevant_informations.hpp"
# include <unordered_map>

namespace PictStock
{
	//typedef Yuni::CString<30, false> KeyString;
    typedef std::unordered_multimap<Private::RelevantInformations, Yuni::String> ValidDirectoriesType;

} // namespace PictStock




#endif /* _PRIVATE_TOOLS_HPP_ */
