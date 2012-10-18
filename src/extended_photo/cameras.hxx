#ifndef CAMERAS_HXX_
# define CAMERAS_HXX_

namespace PictStock
{
namespace ExtendedPhoto
{

	inline const std::unordered_set<TableCameras::Keyword::StringType>& Cameras::keywords() const
	{
		return pKeywords;
	}


	inline void Cameras::addNewTuple(const TupleString& tuple)
	{
		pRows.push_back(tuple);
		pKeywords.insert(std::get<GenericTools::IndexOf<TableCameras::Keyword, Tuple>::value >(tuple));
	}


	template<class StringT>
	inline void Cameras::identifyPhotographerAbbr(
		const StringT& abbreviation,
		Photographer::Ptr photographer) const
	{
		TableCameras::Owner::StringType buf(abbreviation);
		identifyPhotographerAbbr(buf, photographer);
	}


	inline void Cameras::addNewPhotographer(const TablePhotographers::FirstName::StringType& firstName,
		const TablePhotographers::LastName::StringType& lastName,
		const TablePhotographers::Abbreviation::StringType& abbreviation)
	{
		assert(!(!pPhotographersPtr));
		pPhotographersPtr->addNewPhotographer(firstName, lastName, abbreviation);
	}


} // namespace ExtendedPhoto
} // namespace pictStock


#endif /* CAMERAS_HXX_ */
