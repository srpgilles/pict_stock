#ifndef __YUNI_CORE_FWD_H__
# define __YUNI_CORE_FWD_H__

# include <iosfwd> // STL
# ifndef YUNI_OS_MSVC
#	include <bits/stringfwd.h>
# endif

# ifdef None
#	undef None // From X11/X.h
# endif

namespace Yuni
{

	//! Type for a default behavior / policy
	struct YUNI_DECL Default {};

	//! Absence Option
	struct YUNI_DECL None {};


	// Forward declaration for the base class String
	template<uint ChunkSizeT = 128, bool ExpandableT = true>
	class YUNI_DECL CString;


	//! Standard string implementation
	typedef YUNI_DECL CString<> String;

	//! Character Large Object
	typedef YUNI_DECL CString<4096>  Clob;


} // namespace Yuni


//! Convenient typedef for cross namespace lookup
typedef Yuni::String  YString;

/*!
** \brief String adapters
**
** This is a convenient typedef for declaring a string adapter.
** A string adapter allow you to perform all read-only operations
** provided by a string to an arbitrary raw buffer, without copying it.
** This may be extremly useful to reduce memory consumption and to reduce
** some useless memory allocation.
**
** \code
** AnyString s;
** s.adapt("Here is a raw C-string");
** std::cout << "length     : " << s.size() << std::endl;
** std::cout << "find 'raw' : " << s.find("raw") << std::endl;
** \endcode
**
** Using a sub-string as it were a real string :
** \code
** String s = "abcdefghijklmnopqrst";
** AnyString adapter (s.begin() + 2, s.begin() + 9);
** std::cout << adapter << " (size: " << adapter.size() << ")" << std::endl;
** \endcode
*/
typedef ::Yuni::CString<0, true>  AnyString;


#endif // __YUNI_CORE_FWD_H__
