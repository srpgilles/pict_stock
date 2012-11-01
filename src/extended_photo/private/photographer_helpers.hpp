#ifndef __PICT_STOCK__EXTENDED_PHOTO__PRIVATE__PHOTOGRAPHER_HELPERS_HPP_
# define __PICT_STOCK__EXTENDED_PHOTO__PRIVATE__PHOTOGRAPHER_HELPERS_HPP_

# include "../../pict_stock.hpp"

namespace PictStock
{
namespace ExtendedPhoto
{
namespace Private
{

	struct PhotographerInformations
	{
		static const SymbolString Symbol() { return "%P"; }
		static const RegexString Regex() { return ".*"; }
	};


} // namespace Private
} // namespace ExtendedPhoto
} // namespace PictStock



#endif /* __PICT_STOCK__EXTENDED_PHOTO__PRIVATE__PHOTOGRAPHER_HELPERS_HPP_ */
