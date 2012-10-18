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

} // namespace ExtendedPhoto
} // namespace pictStock


#endif /* CAMERAS_HXX_ */
