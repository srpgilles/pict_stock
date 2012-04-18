#ifndef __YUNI_UUID_UUID_HXX__
# define __YUNI_UUID_UUID_HXX__


namespace Yuni
{

	template<class StringT>
	inline UUID::UUID(const StringT& string)
	{
		if (!assign(string))
			clear();
	}


	inline bool UUID::operator ! () const
	{
		return null();
	}


	template<class StringT>
	bool UUID::assign(const StringT& string)
	{
		// YUNI_STATIC_ASSERT(Extension::IntoCString<StringT>::valid, InvalidString);
		const char* cstr = Traits::CString<StringT>::Perform(string);
		const unsigned int length = Traits::Length<StringT,unsigned int>::Value(string);

		if (length >= 36)
		{
			if (Extension::IntoCString<StringT>::zeroTerminated)
			{
				if (initializeFromCString(cstr))
					return true;
			}
			else
			{
				char buffer[64]; // 8 Byte Stack Alignment
				for (unsigned int i = 0; i != 64; ++i)
					buffer[i] = cstr[i];
				buffer[36] = '\0';
				if (initializeFromCString(buffer))
					return true;
			}
		}
		return false;
	}


	template<class StringT>
	UUID& UUID::operator = (const StringT& string)
	{
		if (!assign(string))
			clear();
		return *this;
	}


	inline size_t UUID::hash() const
	{
		// TODO This hash may not be suitable for hashing guids,
		std::size_t r = 0;
		const char* p = pValue.signedcstring;
		for (unsigned int i = 0; i != 16; ++i)
		{
			r = p[i] + (r << 6) + (r << 16) - r;
		}
		return r;
	}



} // namespace Yuni







namespace Yuni
{
namespace Private
{
namespace UUID
{

	class Helper
	{
	public:
		static void WriteToCString(char* cstr, const Yuni::UUID& uuid)
		{
			uuid.writeToCString(cstr);
		}
	};


} // namespace UUID
} // namespace Private
} // namespace Yuni







namespace Yuni
{
namespace Extension
{
namespace CString
{

	template<class CStringT>
	class Append<CStringT, Yuni::UUID>
	{
	public:
		static void Perform(CStringT& s, const Yuni::UUID& rhs)
		{
			const unsigned int currentLength = s.size();
			// writeToCString is guarantee to have 42 chars
			s.reserve(currentLength + 42); // at least 36 + 1 zero-terminated
			Yuni::Private::UUID::Helper::WriteToCString((char*)s.data() + currentLength, rhs);
			s.resize(currentLength + 36); // guid is 36-bytes length
			//s.removeLast();
		}

	}; // class Append


	template<>
	class Into<Yuni::UUID>
	{
	public:
		typedef Yuni::UUID TargetType;
		enum { valid = 1 };

		template<class StringT> static bool Perform(const StringT& s, TargetType& out)
		{
			return out.assign(s);
		}

		template<class StringT> static TargetType Perform(const StringT& s)
		{
			return TargetType(s);
		}

	}; // class Into



} // namespace CString
} // namespace Extension
} // namespace Yuni





// ostream
std::ostream& operator << (std::ostream& out, const Yuni::UUID& rhs);


#endif // __YUNI_UUID_UUID_HXX__
