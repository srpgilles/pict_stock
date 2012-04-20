#ifndef EXTENDED_PHOTO_HXX_
#define EXTENDED_PHOTO_HXX_

namespace SgPhoto
{

	inline bool ExtendedPhoto::problem() const
	{
		return pStatus != epFine;
	}


	inline YString ExtendedPhoto::date() const
	{
		return pStringDate;
	}

} // namespace SgPhoto

#endif /* EXTENDED_PHOTO_HXX_ */
