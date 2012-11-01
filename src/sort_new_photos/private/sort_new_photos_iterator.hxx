#ifndef __PICT_STOCK__SORT_NEW_PHOTOS__PRIVATE__SORT_NEW_PHOTOS_ITERATOR_HXX_
# define __PICT_STOCK__SORT_NEW_PHOTOS__PRIVATE__SORT_NEW_PHOTOS_ITERATOR_HXX_

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

#endif /* __PICT_STOCK__SORT_NEW_PHOTOS__PRIVATE__SORT_NEW_PHOTOS_ITERATOR_HXX_ */
