
#ifndef DATE_HELPERS_HPP_
# define DATE_HELPERS_HPP_

# include "../../pict_stock.hpp"

namespace PictStock
{
namespace Private
{

	struct Year
	{
		static const SymbolString Symbol() { return "%y"; }
		static const RegexString Regex() { return "\\d{4}"; }
	};


	struct Month
	{
		static const SymbolString Symbol() { return "%m"; }
		static const RegexString Regex() { return " \\d{1}|0\\d{1}|10|11|12"; }
	};


	struct Day
	{
		static const SymbolString Symbol() { return "%d"; }
		static const RegexString Regex() { return  "[ |0|1|2]\\d{1}|3[0|1]"; } // no check for adequacy month day (for instance 30 Elements::b is allowed)
	};


	struct Hour
	{
		static const SymbolString Symbol() { return "%H"; }
		static const RegexString Regex() { return  "[ |0|1]\\d{1}|2[0-4]|  "; } // empty allowed
	};


	struct Minute
	{
		static const SymbolString Symbol() { return "%M"; }
		static const RegexString Regex() { return  "[ 0-5]\\d{1}|  "; } // empty is allowed
	};


	struct Second
	{
		static const SymbolString Symbol() { return "%S"; }
		static const RegexString Regex() { return  "[ 0-5]\\d{1}|  "; } // empty is allowed
	};



} // namespace Private
} // namespace PictStock

#endif /* DATE_HELPERS_HPP_ */
