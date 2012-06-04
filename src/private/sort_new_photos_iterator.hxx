#ifndef SORT_NEW_PHOTOS_ITERATOR_HXX_
# define SORT_NEW_PHOTOS_ITERATOR_HXX_

namespace PictStock
{
namespace Private
{


	inline void SortNewPhotosIterator::picturesToProcess(OrderedPhotos& out) const
	{
		out = pPicturesToProcess;
	}

} // namespace Private
} // namespace PictStock

#endif /* SORT_NEW_PHOTOS_ITERATOR_HXX_ */
