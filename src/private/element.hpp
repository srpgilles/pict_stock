#ifndef PICT_STOCK_PRIVATE_TRAITS_TRAITS_HPP
# define PICT_STOCK_PRIVATE_TRAITS_TRAITS_HPP

# include <yuni/core/string.h>

namespace PictStock
{
namespace Private
{


	class YUNI_DECL Element
	{
	public:

		typedef Yuni::CString<4, false> SymbolString;
		typedef Yuni::CString<50, false> RegexString;

		//! Smart pointer most adapted for the class
		typedef Yuni::SmartPtr<Element> Ptr;

		//! Container most adapted for the class
		typedef std::vector<Ptr> Vector;

		//! Constructor
		Element(const SymbolString& symbol, const RegexString& regex);

		//! Regex
		RegexString regex() const;

		//! Symbol
		SymbolString symbol() const;


	private:

		//! Symbol used in user-defined expression. For instance %y for year
		SymbolString pSymbol;

		//! Regex expression to put when looking for the element in an existing path
		RegexString pRegex;
	};


} // namespace Private
} // namespace PictStock

#endif // PICT_STOCK_PRIVATE_TRAITS_TRAITS_HPP
