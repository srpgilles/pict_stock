#ifndef __PICT_STOCK__PICT_STOCK_HPP_
# define __PICT_STOCK__PICT_STOCK_HPP_

# include <yuni/core/logs.h>
# include <yuni/core/noncopyable.h>

typedef Yuni::Logs::Logger<> LoggingFacility;

namespace PictStock
{

	namespace ExtendedPhoto
	{
		// Forward declaration
		class PathInformations;
	}

    typedef std::multimap<ExtendedPhoto::PathInformations, YString>
    	ValidDirectoriesType;

    typedef Yuni::CString<4, false> SymbolString;
    typedef Yuni::CString<30, true> RegexString;

} // namespace PictStock


#endif /* __PICT_STOCK__PICT_STOCK_HPP_ */
