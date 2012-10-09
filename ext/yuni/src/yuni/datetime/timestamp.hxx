#ifndef __YUNI_DATETIME_TIMESTAMP_HXX__
# define __YUNI_DATETIME_TIMESTAMP_HXX__


namespace Yuni
{
namespace Private
{
namespace DateTime
{

	// forward declaration
	char* FormatStringDynBuffer(uint size, const char* format, sint64 timestamp);


} // namespace DateTime
} // namespace Private
} // namespace Yuni


namespace Yuni
{
namespace DateTime
{

	template<class StringT1, class StringT2>
	bool TimestampToString(StringT1& out, const StringT2& format, Timestamp timestamp,
		bool emptyBefore)
	{
		YUNI_STATIC_ASSERT(Traits::CString<StringT1>::valid, T1_InvalidType);
		YUNI_STATIC_ASSERT(Traits::Length<StringT1>::valid,  T1_InvalidTypeSize);
		YUNI_STATIC_ASSERT(Traits::CString<StringT2>::valid, T2_InvalidType);
		YUNI_STATIC_ASSERT(Traits::Length<StringT2>::valid,  T2_InvalidTypeSize);

		if (!Traits::CString<StringT2>::zeroTerminated)
		{
			// System API requires null-terminated strings
			String copyformat = format;
			return TimestampToString(out, copyformat, timestamp, emptyBefore);
		}
		else
		{
			if (emptyBefore)
				out.clear();

			uint guesssize = Traits::Length<StringT2,uint>::Value(format);
			if (!guesssize)
				return true;

			const char* cstrformat = Traits::CString<StringT2>::Perform(format);
			guesssize += 128; // arbitrary value
			char* buffer = Yuni::Private::DateTime::FormatStringDynBuffer(guesssize, cstrformat, timestamp);
			if (buffer)
			{
				out += (const char*) buffer;
				delete[] buffer;
				return true;
			}
			return false;
		}
	}





} // namespace DateTime
} // namespace Yuni

#endif // __YUNI_DATETIME_TIMESTAMP_H__
