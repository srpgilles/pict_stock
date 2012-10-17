#ifndef SORT_NEW_PHOTOS_ITERATOR_HXX_
# define SORT_NEW_PHOTOS_ITERATOR_HXX_

namespace PictStock
{
namespace SortNewPhotos
{
namespace Private
{

	inline void SortNewPhotosIterator::picturesToProcess(ExtendedPhoto::OrderedPhotos& out) const
	{
		out = pPicturesToProcess;
	}

} // namespace Private
} // namespace SortNewPhotos
} // namespace PictStock

#endif /* SORT_NEW_PHOTOS_ITERATOR_HXX_ */
