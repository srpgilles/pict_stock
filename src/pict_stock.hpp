#ifndef _PICT_STOCK_HPP_
# define _PICT_STOCK_HPP_

# include <yuni/core/logs.h>

typedef Yuni::Logs::Logger<> LoggingFacility;

namespace PictStock
{
	// Forward declaration
	class PathInformations;

    typedef std::multimap<PathInformations, YString>
    	ValidDirectoriesType;
} // namespace PictStock


#endif /* _PICT_STOCK_HPP_ */
