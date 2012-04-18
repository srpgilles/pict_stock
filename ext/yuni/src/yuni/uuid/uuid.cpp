
#include "uuid.h"
#ifndef YUNI_OS_WINDOWS
# include <uuid/uuid.h>
#else
# include <objbase.h>
#endif
#include "../core/string.h"
#include <cassert>



namespace Yuni
{

	void UUID::generate()
	{
		assert(sizeof(StorageType) == 16 && "Invalid storage size for uuid");

		# ifndef YUNI_OS_WINDOWS
		assert(sizeof(uuid_t) == 16);
		uuid_generate(pValue.cstring);
		# else
		if (S_OK != ::CoCreateGuid((::GUID*)pValue.cstring)) // Sadly, the call can fail
			clear();
		# endif
	}


	void UUID::writeToCString(char* cstring) const
	{
		assert(cstring && "invalid pointer");
		# ifndef YUNI_OS_WINDOWS
		uuid_unparse(pValue.cstring, cstring);
		# else
		// StringFromGUID2 returns a string enclosed in braces
		// Anything less than 39 would make the call fail
		wchar_t buffer[39];
		::StringFromGUID2(*(::GUID*)pValue.cstring, buffer, 39);
		// Convert to non-wide string, and cut the prepended and appended braces
#ifdef YUNI_OS_MINGW
		// we should have something like {000000000-0000-0000-0000-00000000000} in buffer
		if (::wcstombs(cstring, buffer + 1, 36))
			::strncpy(cstring, "000000000-0000-0000-0000-00000000000", 36);
#else
		size_t converted = 0;
		// we should have something like {000000000-0000-0000-0000-00000000000} in buffer
		if (::wcstombs_s(&converted, cstring, 42, buffer + 1, 36))
			::strcpy_s(cstring, 36, "000000000-0000-0000-0000-00000000000");
#endif // YUNI_OS_MINGW
		else
		{
			// The guid produced on Windows is uppercase
			for (unsigned int i = 0; i != 36; ++i)
				cstring[i] = static_cast<char>(String::ToLower(cstring[i]));
		}
		// Do not forget the null terminator
		cstring[36] = '\0';
		# endif
	}


	bool UUID::initializeFromCString(const char* cstring)
	{
		# ifndef YUNI_OS_WINDOWS
		// Why uuid_parse takes a char* and not a const char* ??
		return !uuid_parse(const_cast<char*>(cstring), pValue.cstring);
		# else
		// Stop complaining, the Windows implementation is way worse.
		char* cstring_noconst = const_cast<char*>(cstring);
		unsigned char* cstring_unsigned = (unsigned char*)(cstring_noconst);
		return RPC_S_OK == ::UuidFromStringA(cstring_unsigned, (::GUID*)pValue.cstring);
		# endif
	}


	bool UUID::null() const
	{
		return !pValue.n32[0] && !pValue.n32[1] && !pValue.n32[2] && !pValue.n32[3];
	}


	void UUID::clear()
	{
		pValue.n32[0] = 0;
		pValue.n32[1] = 0;
		pValue.n32[2] = 0;
		pValue.n32[3] = 0;
	}


	UUID::UUID()
	{
		pValue.n32[0] = 0;
		pValue.n32[1] = 0;
		pValue.n32[2] = 0;
		pValue.n32[3] = 0;
	}


	UUID::UUID(const UUID& rhs)
	{
		pValue.n32[0] = rhs.pValue.n32[0];
		pValue.n32[1] = rhs.pValue.n32[1];
		pValue.n32[2] = rhs.pValue.n32[2];
		pValue.n32[3] = rhs.pValue.n32[3];
	}


	UUID::UUID(Flag flag)
	{
		switch (flag)
		{
			case fGenerate: generate(); break;
			case fNull: clear(); break;
		}
	}


	UUID& UUID::operator = (const UUID& rhs)
	{
		pValue.n32[0] = rhs.pValue.n32[0];
		pValue.n32[1] = rhs.pValue.n32[1];
		pValue.n32[2] = rhs.pValue.n32[2];
		pValue.n32[3] = rhs.pValue.n32[3];
		return *this;
	}


	bool UUID::operator == (const UUID& rhs) const
	{
		return (pValue.n32[0] == rhs.pValue.n32[0])
			&& (pValue.n32[1] == rhs.pValue.n32[1])
			&& (pValue.n32[2] == rhs.pValue.n32[2])
			&& (pValue.n32[3] == rhs.pValue.n32[3]);
	}


	bool UUID::operator != (const UUID& rhs) const
	{
		return (pValue.n32[0] != rhs.pValue.n32[0])
			|| (pValue.n32[1] != rhs.pValue.n32[1])
			|| (pValue.n32[2] != rhs.pValue.n32[2])
			|| (pValue.n32[3] != rhs.pValue.n32[3]);
	}


	bool UUID::operator < (const UUID& rhs) const
	{
		return pValue.n32[3] < rhs.pValue.n32[3]
			&& pValue.n32[2] < rhs.pValue.n32[2]
			&& pValue.n32[1] < rhs.pValue.n32[1]
			&& pValue.n32[0] < rhs.pValue.n32[0];
	}



} // namespace Yuni



std::ostream& operator << (std::ostream& out, const Yuni::UUID& rhs)
{
	// WriteToCString is guarantee to have 42 chars
	char cstring[42];
	Yuni::Private::UUID::Helper::WriteToCString(cstring, rhs);
	out.write(cstring, 36);
	return out;
}



