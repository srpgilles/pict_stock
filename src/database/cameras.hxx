#ifndef CAMERAS_HXX_
# define CAMERAS_HXX_

namespace PictStock
{
namespace ExtendedPhoto
{

	inline const std::unordered_set<TableCameras::Keyword::WrappedType>& Cameras::keywords() const
	{
		return pKeywords;
	}


	inline void Cameras::addNewTuple(const TupleWrappedType& tuple)
	{
		pRows.push_back(tuple);
		pKeywords.insert(std::get<GenericTools::IndexOf<TableCameras::Keyword, Tuple>::value >(tuple));
	}


	template<class StringT>
	inline void Cameras::identifyPhotographerAbbr(
		const StringT& abbreviation,
		Photographer::Ptr& photographer) const
	{
		TableCameras::Owner::WrappedType buf(abbreviation);
		identifyPhotographerAbbr(buf, photographer);
	}


	inline void Cameras::addNewPhotographer(const TablePhotographers::FirstName::WrappedType& firstName,
		const TablePhotographers::LastName::WrappedType& lastName,
		const TablePhotographers::Abbreviation::WrappedType& abbreviation)
	{
		assert(!(!pPhotographersPtr));
		pPhotographersPtr->addNewPhotographer(firstName, lastName, abbreviation);
	}


} // namespace ExtendedPhoto
} // namespace pictStock


#endif /* CAMERAS_HXX_ */
