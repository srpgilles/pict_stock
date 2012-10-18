
#include <sys/stat.h>
#include "../file.h"
#include "../directory.h"
#include <ctype.h>
#include <string.h>
#include "../../core/string/wstring.h"

#ifndef YUNI_OS_WINDOWS
# include <unistd.h>
#endif

#ifndef YUNI_OS_WINDOWS
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif


namespace Yuni
{
namespace IO
{
namespace File
{


	# ifdef YUNI_OS_WINDOWS

	bool Size(const AnyString& filename, uint64& value)
	{
		unsigned int len = filename.size();
		if (!len)
		{
			value = 0u;
			return false;
		}

		const char* const p = filename.c_str();

		if (p[len - 1] == '\\' || p[len - 1] == '/')
			--len;

		// Driver letters
		if (len == 2 && p[1] == ':')
		{
			value = 0u;
			return true;
		}

		String  norm;
		Yuni::IO::Normalize(norm, p, len);
		// Conversion into wchar_t
		Private::WString<true> wstr(norm);
		if (wstr.empty())
		{
			value = 0u;
			return false;
		}

		HANDLE hndl = CreateFileW(wstr.c_str(), 0, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hndl == INVALID_HANDLE_VALUE)
		{
			value = 0u;
			return false;
		}
		LARGE_INTEGER v;
		if (!GetFileSizeEx(hndl, &v))
		{
			CloseHandle(hndl);
			value = 0u;
			return false;
		}
		value = (uint64) v.QuadPart;

		CloseHandle(hndl);
		return true;
	}

	# else

	bool Size(const AnyString& filename, uint64& value)
	{
		struct stat results;
		if (not filename.empty() && stat(filename.c_str(), &results) == 0)
		{
			value = (uint64) results.st_size;
			return true;
		}
		value = 0u;
		return false;
	}

	# endif



	Yuni::IO::Error Delete(const AnyString& filename)
	{
		// DeleteFile is actually a macro and will be replaced by DeleteFileW
		// with Visual Studio. Consequently we can not use the word DeleteFile.....

		if (!filename)
			return Yuni::IO::errUnknown;

		# ifndef YUNI_OS_WINDOWS

		return (unlink(filename.c_str()))
			? Yuni::IO::errUnknown
			: Yuni::IO::errNone;

		# else

		const char* const p = filename.c_str();
		unsigned int len = filename.size();

		if (p[len - 1] == '\\' || p[len - 1] == '/')
			--len;

		// Driver letters
		if (len == 2 && p[1] == ':')
			return Yuni::IO::errBadFilename;

		String  norm;
		Yuni::IO::Normalize(norm, p, len);
		// Conversion into wchar_t
		Private::WString<true> wstr(norm);
		if (wstr.empty())
			return Yuni::IO::errUnknown;
		wstr.replace('/', '\\');

		return (DeleteFileW(wstr.c_str()))
			? Yuni::IO::errNone
			: Yuni::IO::errUnknown;
		# endif
	}




	# ifdef YUNI_OS_WINDOWS

	/*
	bool GetLastWriteTime(HANDLE hFile)
	{
		FILETIME ftCreate, ftAccess, ftWrite;
		SYSTEMTIME stUTC, stLocal;

		// Retrieve the file times for the file.
		if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
			return false;

		// Convert the last-write time to local time.
		if (!FileTimeToSystemTime(&ftWrite, &stUTC))
			return false;
		if (!SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal))
			return false;

		return true;
	}
	*/

	# endif



	sint64 LastModificationTime(const AnyString& filename)
	{
		# ifdef YUNI_OS_WINDOWS

		Private::WString<> wfilenm(filename);
		if (wfilenm.empty())
			return 0;
		HANDLE hFile = CreateFileW(wfilenm.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return 0;

		FILETIME ftCreate, ftAccess, ftWrite;
		// Retrieve the file times for the file.
		if (GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		{
			CloseHandle(hFile);

			LARGE_INTEGER date, adjust;
			date.HighPart = ftWrite.dwHighDateTime;
			date.LowPart = ftWrite.dwLowDateTime;

			// 100-nanoseconds = milliseconds * 10000
			adjust.QuadPart = 11644473600000 * 10000;

			// removes the diff between 1970 and 1601
			date.QuadPart -= adjust.QuadPart;

			// converts back from 100-nanoseconds to seconds
			return date.QuadPart / 10000000;
		}

		CloseHandle(hFile);
		return 0;

		# else // UNIX

		struct stat st;
		if (!stat(filename.c_str(), &st))
			return st.st_mtime;
		return 0;

		# endif
	}




} // namespace File
} // namespace IO
} // namespace Yuni

