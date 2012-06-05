#ifndef SORT_NEW_PHOTOS_ITERATOR_HXX_
# define SORT_NEW_PHOTOS_ITERATOR_HXX_

namespace PictStock
{


	inline void SortNewPhotosIterator::picturesToProcess(OrderedPhotos& out) const
	{
		out = pPicturesToProcess;
	}

} // namespace PictStock

#endif /* SORT_NEW_PHOTOS_ITERATOR_HXX_ */
