#include "relevant_informations.hpp"

namespace PictStock
{
namespace Private
{

	RelevantInformations::RelevantInformations(LoggingFacility& logs)
		: logs(logs)
	{
		// Define it later

	}


	Yuni::CString<10, false> RelevantInformations::value(unsigned int natureIndex) const
	{
		assert(!(!pDatePtr));
		const Date& date = *pDatePtr;

		switch(natureIndex)
		{
		case Elements::year:
			return date.year;
		case Elements::month:
			return date.month;
		case Elements::day:
			return date.day;
		case Elements::hour:
			return date.hour;
		case Elements::minute:
			return date.minute;
		case Elements::second:
			return date.second;
		case Elements::photographer:
			{
				assert(!(!pPhotographerPtr));
				return pPhotographerPtr->abbr();
			}
		case Elements::size:
			assert(false && "Should never be invoked");
			logs.fatal("RelevantInformations::value() should never be invoked with very last index");
			exit(EXIT_FAILURE);
			break;
		}// switch

		return ""; // to avoid compilation warning
	}



} // namespace Private
} // namespace PictStock
