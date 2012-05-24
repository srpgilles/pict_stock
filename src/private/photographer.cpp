#include "photographer.hpp"

namespace PictStock
{
namespace Private
{

	Photographer::Photographer(const YString& name,
		const YString& abbrev,
		const ExifDataType& cameras)
		: pName(name),
		  pAbbrev(abbrev),
		  pCameras(cameras)
	{ }

} // namespace Private
} // namespace PictStock
