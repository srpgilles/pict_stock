#ifndef _PRIVATE_TOOLS_HPP_
# define _PRIVATE_TOOLS_HPP_

# include "../extended_photo/path_informations.hpp"
# include <map>

namespace PictStock
{
	//typedef Yuni::CString<30, false> KeyString;
    typedef std::multimap<Private::PathInformations, YString>
    	ValidDirectoriesType;

} // namespace PictStock




#endif /* _PRIVATE_TOOLS_HPP_ */
