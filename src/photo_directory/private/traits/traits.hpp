#ifndef PICT_STOCK_PRIVATE_TRAITS_TRAITS_HPP
# define PICT_STOCK_PRIVATE_TRAITS_TRAITS_HPP

# include <yuni/core/string.h>

namespace PictStock
{
namespace Elements // waiting MSVC implement enum class...
{
	enum Nature
	{
		year = 0,
		month,
		day,
		hour,
		minute,
		second,
		photographer,
		size
	};
} // namespace Elements


namespace Traits
{

	typedef Yuni::CString<4, false> SymbolString;
	typedef Yuni::CString<30, true> RegexString;

	struct Element
	{
		Element(const unsigned int nature);

		typedef Yuni::SmartPtr<Element> Ptr;
		typedef std::vector<Ptr> Vector;

		virtual const SymbolString symbol() const  = 0;
		virtual const RegexString regex() const = 0;

		unsigned int nature; // conversion in unsigned int of an item from enum Elements::Nature
	};



	struct Year : public Element
	{
		Year();

		static const SymbolString Symbol() { return "%y"; }
		static const RegexString Regex() { return "\\d{4}"; }

		virtual const SymbolString symbol() const { return Symbol(); }
		virtual const RegexString regex() const { return Regex(); }

		enum { index = Elements::year };
	};


	struct Month : public Element
	{
		Month();

		static const SymbolString Symbol() { return "%m"; }
		static const RegexString Regex() { return " \\d{1}|0\\d{1}|10|11|12"; }

		virtual const SymbolString symbol() const { return Symbol(); }
		virtual const RegexString regex() const { return Regex(); }

		enum { index = Elements::month };
	};


	struct Day : public Element
	{
		Day();

		static const SymbolString Symbol() { return "%d"; }
		static const RegexString Regex() { return  "[ |0|1|2]\\d{1}|3[0|1]"; } // no check for adequacy month day (for instance 30 Elements::b is allowed)

		virtual const SymbolString symbol() const { return Symbol(); }
		virtual const RegexString regex() const { return Regex(); }

		enum { index = Elements::day };
	};


	struct Hour : public Element
	{
		Hour();

		static const SymbolString Symbol() { return "%H"; }
		static const RegexString Regex() { return  "[ |0|1]\\d{1}|2[0-4]|  "; } // empty allowed

		virtual const SymbolString symbol() const { return Symbol(); }
		virtual const RegexString regex() const { return Regex(); }

		enum { index = Elements::hour };
	};


	struct Minute : public Element
	{
		Minute();

		static const SymbolString Symbol() { return "%M"; }
		static const RegexString Regex() { return  "[ 0-5]\\d{1}|  "; } // empty is allowed

		virtual const SymbolString symbol() const { return Symbol(); }
		virtual const RegexString regex() const { return Regex(); }

		enum { index = Elements::minute };
	};


	struct Second : public Element
	{
		Second();

		static const SymbolString Symbol() { return "%S"; }
		static const RegexString Regex() { return  "[ 0-5]\\d{1}|  "; } // empty is allowed

		virtual const SymbolString symbol() const { return Symbol(); }
		virtual const RegexString regex() const { return Regex(); }

		enum { index = Elements::second };
	};


	struct Photographer : public Element
	{
		Photographer();

		static const SymbolString Symbol() { return "%P"; }
		static const RegexString Regex() { return ".*"; }

		virtual const SymbolString symbol() const { return Symbol(); }
		virtual const RegexString regex() const { return Regex(); }

		enum { index = Elements::photographer };
	};


} // namespace Traits
} // namespace PictStock

#endif // PICT_STOCK_PRIVATE_TRAITS_TRAITS_HPP
