#include "photographer.hpp"

namespace SgPhoto
{
	Photographer::Photographer(const YString& name,
		const YString& abbrev,
		const ExifDataType& cameras)
		: pName(name),
		  pAbbrev(abbrev),
		  pCameras(cameras)
	{ }

} // namespace SgPhoto
