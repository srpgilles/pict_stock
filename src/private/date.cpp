#ifdef USE_BOOST_REGULAR_EXPR
# include <boost/regex.hpp>
#endif //USE_BOOST_REGULAR_EXPR

#include "../pict_stock.hpp"
#include "date.hpp"
#include "traits/traits.hpp"

namespace PictStock
{
namespace Private
{
	namespace
	{
		static const YString expression =
			YString("\\A")
			<< Traits::Year::Regex()
			<< ':' // separator
			<< Traits::Month::Regex()
			<< ':' // separator
			<< Traits::Day::Regex()
			<< ' ' // separator
			<< Traits::Hour::Regex()
			<< ':' // separator
			<< Traits::Minute::Regex()
			<< ':' // separator
			<< Traits::Second::Regex()
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




