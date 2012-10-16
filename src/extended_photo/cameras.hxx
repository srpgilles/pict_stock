#ifndef CAMERAS_HXX_
# define CAMERAS_HXX_

namespace PictStock
{

	inline const std::vector<NSCameras::Private::Keyword::StringType>& Cameras::keywords() const
	{
		return pKeywords;
	}

} // namespace pictStock


#endif /* CAMERAS_HXX_ */
