
#ifndef DATE_HELPERS_HPP_
# define DATE_HELPERS_HPP_

# include "../../pict_stock.hpp"

namespace PictStock
{
namespace ExtendedPhoto
{
namespace Private
{

	struct Year
	{
		static const SymbolString Symbol() { return "%Y"; }
		static const RegexString Regex() { return "\\d{4}"; }
	};


	struct Month
	{
		static const SymbolString Symbol() { return "%m"; }
		static const RegexString Regex()
		{
			return "[0 ][1-9]|" // two ways of writing from 1 to 9
				   "10|11|12"; // three remaining cases
		}
	};


	struct Day
	{
		static const SymbolString Symbol() { return "%d"; }
		static const RegexString Regex()
		{
			return  "[0 ][1-9]|" // two ways of writing from 1 to 9
					"[12]\\d{1}|" // 10-29
					"30|31"; // 30-31
		}
	};


	struct Hour
	{
		static const SymbolString Symbol() { return "%H"; }

		static const RegexString Regex()
		{
			return  "[ 01]\\d{1}|" // 0 - 19
					"2[0-3]|" // 20-23
					"  "; // empty allowed
		}
	};


	struct Minute
	{
		static const SymbolString Symbol() { return "%M"; }
		static const RegexString Regex()
		{
			//return "\\d{2}|  ";
			return  "[0-5]\\d{1}|" // 0-59"
					"  "; // allow empty sequence
		}
	};


	struct Second
	{
		static const SymbolString Symbol() { return "%S"; }
		static const RegexString Regex() { return Minute::Regex(); }
	};

} // namespace Private
} // namespace ExtendedPhoto
} // namespace PictStock

#endif /* DATE_HELPERS_HPP_ */
