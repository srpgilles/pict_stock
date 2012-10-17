#ifndef PHOTOGRAPHERS_HXX_
# define PHOTOGRAPHERS_HXX_

namespace PictStock
{
namespace ExtendedPhoto
{

	inline const std::vector<NSExtendedPhoto::Private::Keyword::StringType>& Cameras::keywords() const
	{
		return pKeywords;
	}

} // namespace ExtendedPhoto
} // namespace pictStock


#endif /* PHOTOGRAPHERS_HXX_ */
