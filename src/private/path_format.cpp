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
	}

}// namespace Private
}// namespace PictStock
