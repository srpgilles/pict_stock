#include "date.hpp"
#include "path_format.hpp"
#include <yuni/io/file.h>


using namespace Yuni;

namespace PictStock
{
namespace Private
{


	PathFormat::PathFormat(LoggingFacility& logs, const AnyString& format)
		: logs(logs)
	{
		String path, filename;

		// Split the path and the filename
		IO::ExtractFilePath(path, format);

		if (path.empty())
			filename = format;
		else
			IO::ExtractFileName(filename, format);

		logs.notice("PATH = ") << path;
		logs.notice("FILE = ") << filename;

		path.replace("%y", Date::yearRegex);
		path.replace("%m", Date::monthRegex);
		path.replace("%d", Date::dayRegex);
		path.replace("%H", Date::hourRegex);
		path.replace("%M", Date::minuteRegex);
		path.replace("%S", Date::secondRegex);
		//path.replace("%P",

		pRegExFolder = boost::regex(path.c_str());

		logs.notice(path);





//		%y for the year
//			** 			%m for month
//			** 			%d for day
//			** 			%H for hour
//			** 			%M for minutes
//			** 			%S for seconds
//			** 			%P for the photographer

	}

}// namespace Private
}// namespace PictStock
