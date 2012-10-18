#ifndef PATH_INFORMATIONS_HXX_
# define PATH_INFORMATIONS_HXX_

namespace PictStock
{
namespace ExtendedPhoto
{

	inline void PathInformations::setPhotographer(const Photographer::Ptr& photographer)
	{
		pPhotographerPtr = photographer;
	}


	inline void PathInformations::setDate(const Date& date)
	{
		pDate = date;
	}

	template<>
	inline void PathInformations::setElement<Private::Year>(const AnyString& input)
	{
		pDate.set<Private::Year>(input);
	}

	template<>
	inline void PathInformations::setElement<Private::Month>(const AnyString& input)
	{
		pDate.set<Private::Month>(input);
	}

	template<>
	inline void PathInformations::setElement<Private::Day>(const AnyString& input)
	{
		pDate.set<Private::Day>(input);
	}

	template<>
	inline void PathInformations::setElement<Private::Hour>(const AnyString& input)
	{
		pDate.set<Private::Hour>(input);
	}

	template<>
	inline void PathInformations::setElement<Private::Minute>(const AnyString& input)
	{
		pDate.set<Private::Minute>(input);
	}

	template<>
	inline void PathInformations::setElement<Private::Second>(const AnyString& input)
	{
		pDate.set<Private::Second>(input);
	}

	template<>
	inline void PathInformations::setElement<Private::PhotographerInformations>(const AnyString& input)
	{
		pCameras.identifyPhotographerAbbr(input, pPhotographerPtr); // throws if failure
	}

	template<>
	inline void PathInformations::setElement<Private::Year>(const PathInformations& input)
	{
		pDate.copy<Private::Year>(input.date());
	}

	template<>
	inline void PathInformations::setElement<Private::Month>(const PathInformations& input)
	{
		pDate.copy<Private::Month>(input.date());
	}

	template<>
	inline void PathInformations::setElement<Private::Day>(const PathInformations& input)
	{
		pDate.copy<Private::Day>(input.date());
	}

	template<>
	inline void PathInformations::setElement<Private::Hour>(const PathInformations& input)
	{
		pDate.copy<Private::Hour>(input.date());
	}

	template<>
	inline void PathInformations::setElement<Private::Minute>(const PathInformations& input)
	{
		pDate.copy<Private::Minute>(input.date());
	}

	template<>
	inline void PathInformations::setElement<Private::Second>(const PathInformations& input)
	{
		pDate.copy<Private::Second>(input.date());
	}

	template<>
	inline void PathInformations::setElement<Private::PhotographerInformations>(const PathInformations& input)
	{
		pPhotographerPtr = input.pPhotographerPtr; // valid even with nullptr
	}

	template<>
	inline PathInformations::ElementString PathInformations::getElement<Private::Year>() const
	{
		Date::DateString out;
		pDate.get<Private::Year>(out);
		return out;
	}

	template<>
	inline PathInformations::ElementString PathInformations::getElement<Private::Month>() const
	{
		Date::DateString out;
		pDate.get<Private::Month>(out);
		return out;
	}

	template<>
	inline PathInformations::ElementString PathInformations::getElement<Private::Day>() const
	{
		Date::DateString out;
		pDate.get<Private::Day>(out);
		return out;
	}

	template<>
	inline PathInformations::ElementString PathInformations::getElement<Private::Hour>() const
	{
		Date::DateString out;
		pDate.get<Private::Hour>(out);
		return out;
	}

	template<>
	inline PathInformations::ElementString PathInformations::getElement<Private::Minute>() const
	{
		Date::DateString out;
		pDate.get<Private::Minute>(out);
		return out;
	}

	template<>
	inline PathInformations::ElementString PathInformations::getElement<Private::Second>() const
	{
		Date::DateString out;
		pDate.get<Private::Second>(out);
		return out;
	}

	template<>
	inline PathInformations::ElementString PathInformations::getElement<Private::PhotographerInformations>() const
	{
		if (!(!pPhotographerPtr))
			return pPhotographerPtr->abbreviation();

		return "UNK";
	}

	inline bool operator!=(const PathInformations& lhs, const PathInformations& rhs)
	{
		return !(operator == (lhs, rhs));
	}


	inline const Date& PathInformations::date() const
	{
		return pDate;
	}


} // namespace ExtendedPhoto
} // namespace PictStock


#endif /* PATH_INFORMATIONS_HXX_ */
