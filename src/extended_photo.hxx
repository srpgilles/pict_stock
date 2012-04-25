#ifndef EXTENDED_PHOTO_HXX_
#define EXTENDED_PHOTO_HXX_

namespace SgPhoto
{

	inline bool ExtendedPhoto::problem() const
	{
		return pStatus != epFine;
	}


	inline DateString ExtendedPhoto::date() const
	{
		return pStringDate;
	}


	inline HourString ExtendedPhoto::time() const
	{
		return pStringTime;
	}


	inline YString ExtendedPhoto::originalPath() const
	{
		return pOriginalPath;
	}





} // namespace SgPhoto

#endif /* EXTENDED_PHOTO_HXX_ */
