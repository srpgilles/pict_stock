#ifndef __PICT_STOCK__DATABASE__CAMERAS_HXX_
# define __PICT_STOCK__DATABASE__CAMERAS_HXX_

namespace PictStock
{
namespace Database
{

	inline const std::unordered_set<TableCameras::Keyword::WrappedType>& Cameras::keywords() const
	{
		return pKeywords;
	}


	inline void Cameras::addNewTuple(const TupleWrappedType& tuple)
	{
		pRows.push_back(tuple);
        pKeywords.insert(std::get<GenericTools::Tuple::IndexOf<TableCameras::Keyword, Tuple>::value >(tuple));
	}


	template<class StringT>
	inline void Cameras::identifyPhotographerAbbr(
		const StringT& abbreviation,
		Photographer::Ptr& photographer) const
	{
		TableCameras::Owner::WrappedType buf(abbreviation);
		identifyPhotographerAbbr(buf, photographer);
	}




} // namespace Database
} // namespace pictStock


#endif /* __PICT_STOCK__DATABASE__CAMERAS_HXX_ */
