#ifndef PHOTOGRAPHER_HXX_
# define PHOTOGRAPHER_HXX_

namespace SgPhoto
{

	inline YString Photographer::name() const
	{
		return pName;
	}


	inline YString Photographer::abbr() const
	{
		return pAbbrev;
	}


	inline const Photographer::ExifDataType& Photographer::exifData() const
	{
		return pCameras;
	}


} // namespace SgPhoto

#endif /* PHOTOGRAPHER_HXX_ */
