#ifndef PICT_STOCK_PRIVATE_TRAITS_TRAITS_HPP
# define PICT_STOCK_PRIVATE_TRAITS_TRAITS_HPP

# include <yuni/core/string.h>

namespace PictStock
{
namespace Private
{
namespace Traits
{
	enum
	{
		feYear = 0,
		feMonth,
		feDay,
		feHour,
		feMinute,
		feSecond,
		fePhotographer,
		feSize
	};

	typedef Yuni::CString<4, false> SymbolString;
	typedef Yuni::CString<30, true> RegexString;

	struct Year
	{
		static const SymbolString Symbol() { return "%y"; }
		static const RegexString Regex() { return "\\d{4}"; }
		enum { index = feYear };
	};

	struct Month
	{
		static const SymbolString Symbol() { return "%m"; }
		static const RegexString Regex() { return "( \\d{1}|0\\d{1}|10|11|12)"; }
		enum { index = feMonth };
	};

	struct Day
	{
		static const SymbolString Symbol() { return "%d"; }
		static const RegexString Regex() { return  "([ |0|1|2]\\d{1}|3[0|1])"; } // no check for adequacy month day (for instance 30 Feb is allowed)
		enum { index = feDay };
	};

	struct Hour
	{
		static const SymbolString Symbol() { return "%H"; }
		static const RegexString Regex() { return  "([ |0|1]\\d{1}|2[0-4]|  )"; } // empty allowed
		enum { index = feHour };
	};

	struct Minute
	{
		static const SymbolString Symbol() { return "%M"; }
		static const RegexString Regex() { return  "([ 0-5]\\d{1}|  )"; } // empty is allowed
		enum { index = feMinute };
	};

	struct Second
	{
		static const SymbolString Symbol() { return "%S"; }
		static const RegexString Regex() { return  "([ 0-5]\\d{1}|  )"; } // empty is allowed
		enum { index = feSecond };
	};


	struct Photographer
	{
		static const SymbolString Symbol() { return "%P"; }
		static const RegexString Regex() { return ".*"; }
		enum { index = fePhotographer };
	};

} // namespace Traits
} // namespace Private
} // namespace PictStock

#endif // PICT_STOCK_PRIVATE_TRAITS_TRAITS_HPP
