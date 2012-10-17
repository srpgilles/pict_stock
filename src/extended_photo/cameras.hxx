#ifndef CAMERAS_HXX_
# define CAMERAS_HXX_

namespace PictStock
{
namespace ExtendedPhoto
{

	inline const std::vector<Private::Keyword::StringType>& Cameras::keywords() const
	{
		return pKeywords;
	}

} // namespace ExtendedPhoto
} // namespace pictStock


#endif /* CAMERAS_HXX_ */
