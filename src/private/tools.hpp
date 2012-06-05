#ifndef _PRIVATE_TOOLS_HPP_
# define _PRIVATE_TOOLS_HPP_

# include "../extended_photo/relevant_informations.hpp"
# include <map>

namespace PictStock
{
	//typedef Yuni::CString<30, false> KeyString;
    typedef std::multimap<Private::RelevantInformations, YString>
    	ValidDirectoriesType;

} // namespace PictStock




#endif /* _PRIVATE_TOOLS_HPP_ */
