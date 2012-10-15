
#ifndef PHOTOGRAPHER_HELPERS_HPP_
# define PHOTOGRAPHER_HELPERS_HPP_

# include "../../pict_stock.hpp"

namespace PictStock
{
namespace Private
{

	struct PhotographerInformations
	{
		static const SymbolString Symbol() { return "%P"; }
		static const RegexString Regex() { return ".*"; }
	};


} // namespace Private
} // namespace PictStock



#endif /* PHOTOGRAPHER_HELPERS_HPP_ */
