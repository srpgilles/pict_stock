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


	bool operator==(const Photographer& lhs, const Photographer& rhs)
	{
		if (lhs.pName != rhs.pName)
			return false;

		if (lhs.pAbbrev != rhs.pAbbrev)
			return false;

		return (lhs.pCameras == rhs.pCameras);
	}


	bool operator!=(const Photographer& lhs, const Photographer& rhs)
	{
		return !(operator == (lhs, rhs));
	}


} // namespace PictStock
