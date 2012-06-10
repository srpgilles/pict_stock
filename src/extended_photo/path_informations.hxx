#ifndef PATH_INFORMATIONS_HXX_
# define PATH_INFORMATIONS_HXX_

namespace PictStock
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
		pDate.year = input;
	}

	template<>
	inline void PathInformations::setElement<Private::Month>(const AnyString& input)
	{
		pDate.month = input;
	}

	template<>
	inline void PathInformations::setElement<Private::Day>(const AnyString& input)
	{
		pDate.day = input;
	}

	template<>
	inline void PathInformations::setElement<Private::Hour>(const AnyString& input)
	{
		pDate.hour = input;
	}

	template<>
	inline void PathInformations::setElement<Private::Minute>(const AnyString& input)
	{
		pDate.minute = input;
	}

	template<>
	inline void PathInformations::setElement<Private::Second>(const AnyString& input)
	{
		pDate.second = input;
	}

	template<>
	inline void PathInformations::setElement<Private::Photographer>(const AnyString& input)
	{
		std::multimap<std::string, Yuni::String> empty;
		pPhotographerPtr = new Photographer("", input, empty);
	}

	template<>
	inline void PathInformations::setElement<Private::Year>(const PathInformations& input)
	{
		pDate.year = input.pDate.year;
	}

	template<>
	inline void PathInformations::setElement<Private::Month>(const PathInformations& input)
	{
		pDate.month = input.pDate.month;
	}

	template<>
	inline void PathInformations::setElement<Private::Day>(const PathInformations& input)
	{
		pDate.day = input.pDate.day;
	}

	template<>
	inline void PathInformations::setElement<Private::Hour>(const PathInformations& input)
	{
		pDate.hour = input.pDate.hour;
	}

	template<>
	inline void PathInformations::setElement<Private::Minute>(const PathInformations& input)
	{
		pDate.minute = input.pDate.minute;
	}

	template<>
	inline void PathInformations::setElement<Private::Second>(const PathInformations& input)
	{
		pDate.second = input.pDate.second;
	}

	template<>
	inline void PathInformations::setElement<Private::Photographer>(const PathInformations& input)
	{
		pPhotographerPtr = input.pPhotographerPtr; // valid even with nullptr
	}

	template<>
	inline PathInformations::ElementString PathInformations::getElement<Private::Year>() const
	{
		return pDate.year;
	}

	template<>
	inline PathInformations::ElementString PathInformations::getElement<Private::Month>() const
	{
		return pDate.month;
	}

	template<>
	inline PathInformations::ElementString PathInformations::getElement<Private::Day>() const
	{
		return pDate.day;
	}

	template<>
	inline PathInformations::ElementString PathInformations::getElement<Private::Hour>() const
	{
		return pDate.hour;
	}

	template<>
	inline PathInformations::ElementString PathInformations::getElement<Private::Minute>() const
	{
		return pDate.minute;
	}

	template<>
	inline PathInformations::ElementString PathInformations::getElement<Private::Second>() const
	{
		return pDate.second;
	}

	template<>
	inline PathInformations::ElementString PathInformations::getElement<Private::Photographer>() const
	{
		if (!(!pPhotographerPtr))
			return pPhotographerPtr->abbr();

		return "UNK";
	}

	inline bool operator!=(const PathInformations& lhs, const PathInformations& rhs)
	{
		return !(operator == (lhs, rhs));
	}



} // namespace PictStock


#endif /* PATH_INFORMATIONS_HXX_ */
