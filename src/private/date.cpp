#ifdef USE_BOOST_REGULAR_EXPR
# include <boost/regex.hpp>
#endif //USE_BOOST_REGULAR_EXPR

#include "../pict_stock.hpp"
#include "date.hpp"

namespace PictStock
{
namespace Private
{
	namespace
	{
		#ifdef USE_BOOST_REGULAR_EXPR
		/*!
		** \brief Regular expression for date formatting
		**
		** Basically format is YYYY:MM:DD HH:mm:SS
		*/
		static const boost::regex RegexDateFormatting(
			"\\A(\\d{4})" // Year
			":" // separator
			"( \\d{1}|0\\d{1}|10|11|12)" // Month
			":" // separator
			"([ |0|1|2]\\d{1}|3[0|1])" // Day; no check for adequacy month day (for instance 30 Feb is allowed)
			" " // separator
			"([ |0|1]\\d{1}|2[0-4]|  )" // Hour; empty is allowed
			":" // separator
			"([ 0-5]\\d{1}|  )" // Minute; empty is allowed
			":" // separator
			"([ 0-5]\\d{1}|  )" // Seconds; empty is allowed
			"\\z");
		#endif // USE_BOOST_REGULAR_EXPR
	} // anonymous namespace



	bool dateFromExif(LoggingFacility& logs, Date& out, const YString& dateRead)
	{
		#ifdef USE_BOOST_REGULAR_EXPR
		boost::cmatch match;
		if (regex_match(dateRead.c_str(), match, RegexDateFormatting))
		{
			assert(match.size() == 7u && "First one is complete expression, others the sub-expressions");
			out.year = match[1].str();
			out.month = match[2].str();
			out.day = match[3].str();
			out.hour = match[4].str();
			out.minute = match[5].str();
			out.second = match[6].str();
			return true;
		}
		else
		{
			logs.error("Unable to interpret date ") << dateRead << "; does not match expected format";
			return false;
		}
		#else
		YUNI_STATIC_ASSERT(false, TheImplementationIsMissing);
		return false; // to avoid compilation warning
		#endif  // USE_BOOST_REGULAR_EXPR
	}


} // namespace Private
} // namespace PictStock





