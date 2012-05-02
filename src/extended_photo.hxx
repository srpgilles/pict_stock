#ifndef EXTENDED_PHOTO_HXX_
# define EXTENDED_PHOTO_HXX_

namespace PictStock
{

	inline bool ExtendedPhoto::problem() const
	{
		# ifndef NDEBUG
		switch(pStatus)
		{
		case epFine:
			break;
		case epExiv2Problem:
			logs.debug("Exiv2 problem");
			break;
		case epInternalProblem:
			logs.debug("Internal problem");
			break;
		}
		# endif // NDEBUG

		return pStatus != epFine;
	}


	inline DateString ExtendedPhoto::date() const
	{
		assert(pStringDate.size() == 8u);
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





} // namespace PictStock

#endif /* EXTENDED_PHOTO_HXX_ */
