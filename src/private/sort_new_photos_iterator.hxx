#ifndef SORT_NEW_PHOTOS_ITERATOR_HXX_
# define SORT_NEW_PHOTOS_ITERATOR_HXX_

namespace SgPhoto
{
namespace Private
{


	inline void SortNewPhotosIterator::picturesToProcess(
		std::map<DateString, ExtendedPhoto::Vector>& out) const
	{
		out = pPicturesToProcess;
	}

} // namespace Private
} // namespace SgPhoto

#endif /* SORT_NEW_PHOTOS_ITERATOR_HXX_ */
