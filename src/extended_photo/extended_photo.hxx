#ifndef EXTENDED_PHOTO_HXX_
# define EXTENDED_PHOTO_HXX_

namespace PictStock
{
namespace ExtendedPhoto
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



	inline YString ExtendedPhoto::originalPath() const
	{
		return pOriginalPath;
	}


	inline PathInformations::Ptr ExtendedPhoto::informations() const
	{
		return pPathInformations;
	}

} // namespace ExtendedPhoto
} // namespace PictStock

#endif /* EXTENDED_PHOTO_HXX_ */
