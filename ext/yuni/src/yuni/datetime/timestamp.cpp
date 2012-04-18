
#include "timestamp.h"
#include <time.h>
#include <cassert>
#include <iostream>


namespace Yuni
{
namespace Private
{
namespace DateTime
{

	static unsigned int FormatString(char* buffer, unsigned int size, const char* format, sint64 timestamp)
	{
		assert(format && '\0' != *format && "invalid format");

		unsigned int written;

		// Note that unlike on (all?) POSIX systems, in the Microsoft
		// C library localtime() and gmtime() are multi-thread-safe, as the
		// returned pointer points to a thread-local variable. So there is no
		// need for localtime_r() and gmtime_r().

		// \note The variable stdtimestamp is used to ensure the compilation on
		//  32bits platforms

		# ifdef YUNI_OS_MINGW
		// MinGW
		time_t stdtimestamp = static_cast<time_t>(timestamp);
		written = (unsigned int)::strftime(buffer, size, format, ::localtime(&stdtimestamp));
		# else
		struct tm timeinfo;
		#	ifdef YUNI_OS_MSVC
		// Microsoft Visual Studio
		_localtime64_s(&timeinfo, &timestamp);
		written = (unsigned int)::strftime(buffer, size, format, &timeinfo);
		#	else
		// Unixes
		time_t stdtimestamp = static_cast<time_t>(timestamp);
		::localtime_r(&stdtimestamp, &timeinfo);
		written = (unsigned int)::strftime(buffer, size, format, &timeinfo);
		#	endif
		# endif

		if (written && written < size)
			return written;
		return 0;
	}


	char* FormatStringDynBuffer(unsigned int size, const char* format, sint64 timestamp)
	{
		if (!timestamp)
		{
			# ifdef YUNI_OS_MSVC
			timestamp = static_cast<sint64>(::_time64(nullptr));
			# else
			timestamp = static_cast<sint64>(::time(nullptr));
			# endif
		}

		unsigned int tick = 5;
		do
		{
			char* buffer = new char[size];
			if (FormatString(buffer, size, format, timestamp))
				return buffer;
			size += 256;
			delete[] buffer;
		}
		while (--tick);
		return nullptr;
	}




} // namespace DateTime
} // namespace Private
} // namespace Yuni


namespace Yuni
{
namespace DateTime
{

	Timestamp Now()
	{
		# ifdef YUNI_OS_MSVC
		return static_cast<sint64>(::_time64(nullptr));
		# else
		return static_cast<sint64>(::time(nullptr));
		# endif
	}



} // namespace DateTime
} // namespace Yuni


