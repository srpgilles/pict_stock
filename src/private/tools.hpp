#ifndef _PRIVATE_TOOLS_HPP_
#define _PRIVATE_TOOLS_HPP_

namespace SgPhoto
{
	typedef Yuni::CString<4, false> HourString;
    typedef Yuni::CString<8, false> DateString;

    typedef std::map<DateString, std::list<Yuni::String> > ValidDirectoriesType;

}




#endif /* _PRIVATE_TOOLS_HPP_ */
