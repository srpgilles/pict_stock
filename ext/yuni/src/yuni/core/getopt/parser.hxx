#ifndef __YUNI_CORE_GETOPT_PARSER_HXX__
# define __YUNI_CORE_GETOPT_PARSER_HXX__


namespace Yuni
{
namespace GetOpt
{

	template<class U>
	inline void Parser::add(U& var, char shortname, bool visible)
	{
		// The new option
		IOption* option = (visible)
			? (IOption*) new Private::GetOptImpl::Option<U, true>(var, shortname)
			: (IOption*) new Private::GetOptImpl::Option<U, false>(var, shortname);
		// append the new option
		appendShortOption(option, shortname);
	}


	template<class U>
	inline void Parser::addFlag(U& var, char shortname, bool visible)
	{
		// The new option
		IOption* option = (visible)
			? (IOption*) new Private::GetOptImpl::Option<U, true, false>(var, shortname)
			: (IOption*) new Private::GetOptImpl::Option<U, false, false>(var, shortname);
		// append the new option
		appendShortOption(option, shortname);
	}


	template<class U, class S>
	inline void Parser::add(U& var, char shortname, const S& longname, bool visible)
	{
		IOption* option = (visible)
			? (IOption*) new Private::GetOptImpl::Option<U, true>(var, shortname, longname)
			: (IOption*) new Private::GetOptImpl::Option<U, false>(var, shortname, longname);
		// append the new option
		appendOption(option, shortname);
	}


	template<class U, class S, class D>
	inline void Parser::add(U& var, char shortname, const S& longname, const D& description, bool visible)
	{
		IOption* option = (visible)
			? (IOption*) new Private::GetOptImpl::Option<U, true>(var, shortname, longname, description)
			: (IOption*) new Private::GetOptImpl::Option<U, false>(var, shortname, longname, description);
		// append the new option
		appendOption(option, shortname);
	}



	template<class U, class S>
	inline void Parser::addFlag(U& var, char shortname, const S& longname, bool visible)
	{
		// The new option
		IOption* option = (visible)
			? (IOption*) new Private::GetOptImpl::Option<U, true, false>(var, shortname, longname)
			: (IOption*) new Private::GetOptImpl::Option<U, false, false>(var, shortname, longname);
		// append the new option
		appendOption(option, shortname);
	}


	template<class U, class S, class D>
	void Parser::addFlag(U& var, char shortname, const S& longname, const D& description, bool visible)
	{
		// The new option
		IOption* option = (visible)
			? (IOption*) new Private::GetOptImpl::Option<U, true, false>(var, shortname, longname, description)
			: (IOption*) new Private::GetOptImpl::Option<U, false, false>(var, shortname, longname, description);
		// append the new option
		appendOption(option, shortname);
	}


	template<class U>
	inline void Parser::remainingArguments(U& var)
	{
		if (pRemains)
			delete pRemains;
		pRemains = new Private::GetOptImpl::Option<U, false>(var, '\0');
	}


	template<class U>
	void Parser::addParagraph(const U& text)
	{
		pAllOptions.push_back(new Private::GetOptImpl::Paragraph(text));
	}




} // namespace GetOpt
} // namespace Yuni

#endif // __YUNI_CORE_GETOPT_PARSER_H__
