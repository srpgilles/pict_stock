#ifdef USE_BOOST_REGULAR_EXPR
# include <boost/regex.hpp>
#endif //USE_BOOST_REGULAR_EXPR

#include "../pict_stock.hpp"
#include "date.hpp"

namespace PictStock
{
namespace Private
{
namespace Date
{
	namespace
	{
		static const YString expression =
			YString("\\A")
			<< yearRegex
			<< ':' // separator
			<< monthRegex
			<< ':' // separator
			<< dayRegex
			<< ' ' // separator
			<< hourRegex
			<< ':' // separator
			<< minuteRegex
			<< ':' // separator
			<< secondRegex
			<< "\\z";

		#ifdef USE_BOOST_REGULAR_EXPR
		/*!
		** \brief Regular expression for date formatting
		**
		** Basically format is YYYY:MM:DD HH:mm:SS
		*/
		static const boost::regex RegexDateFormatting(expression.c_str());
		#endif // USE_BOOST_REGULAR_EXPR
	} // anonymous namespace



	bool fromExif(LoggingFacility& logs, Date& out, const YString& dateRead)
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

} // namespace Date
} // namespace Private
} // namespace PictStock





