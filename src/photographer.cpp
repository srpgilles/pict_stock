#include "photographer.hpp"

namespace PictStock
{
	Photographer::Photographer(const YString& name,
		const YString& abbrev,
		const ExifDataType& cameras)
		: pName(name),
		  pAbbrev(abbrev),
		  pCameras(cameras)
	{ }

} // namespace PictStock
