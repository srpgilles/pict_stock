#include "element.hpp"


namespace PictStock
{
namespace Private
{

	Element::Element(const SymbolString& symbol, const RegexString& regex)
		: pSymbol(symbol),
		  pRegex(regex)
	{ }


	Element::RegexString Element::regex() const
	{
		return pRegex;
	}

	Element::SymbolString Element::symbol() const
	{
		return pSymbol;
	}





} // namespace Private
} // namespace PictStock
