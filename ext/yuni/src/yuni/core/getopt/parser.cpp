
#include "parser.h"
#include <iostream>
#include <cassert>

// The standard error output is not displayed on Windows
# ifndef YUNI_OS_WINDOWS
#	define STD_CERR  std::cerr
# else
#	define STD_CERR  std::cout
# endif



namespace Yuni
{
namespace Private
{
namespace GetOptImpl
{


	class Context
	{
	public:
		//! typedef for IOption
		typedef Private::GetOptImpl::IOption IOption;
		//! Typedef for Parser
		typedef GetOpt::Parser Parser;
		//! Option list (order given by the user)
		typedef Parser::OptionList OptionList;
		//! All options ordered by their short name
		typedef Parser::OptionsOrderedByShortName OptionsOrderedByShortName;
		//! All options ordered by their long name
		typedef Parser::OptionsOrderedByLongName OptionsOrderedByLongName;

		/*!
		** \brief Types of tokens
		*/
		enum TokenType
		{
			//! A mere parameter
			ttParameter = 0,
			//! A short name of an option (-t or /t on Windows)
			ttShortOption,
			//! A long name of an option (--text or /text on Windows)
			ttLongOption,
		};

	public:

		/*!
		** \brief Get the type a CString (short/long option, parameter...)
		*/
		static TokenType GetTokenType(const char* argv);

	public:
		//! \name Constructor
		//@{
		/*!
		** \brief Constructor
		**
		** \param parser The public parser
		*/
		Context(Parser& parser);
		//@}


		/*!
		** \brief parse the command line
		*/
		bool operator () (int argc, char* argv[]);

	private:
		/*!
		** \brief An option has not been found
		*/
		void optionIsUnknown(const char* name);

		/*!
		** \brief A additional parameter is missing for an option
		*/
		void parameterIsMissing(const char* name);

		/*!
		** \brief Find the additional parameter and add it to the option
		*/
		bool findNextParameter(IOption* option, int argc, char* argv[]);


	private:
		//! The public class for the parser, where all options are stored
		Parser& pParser;
		//! Position in the list of arguments for the current token
		int pTokenIndex;
		//! Position in the list of arguments for parameters
		int pParameterIndex;
		//! The option found
		IOption* option;
		//! The current argument
		const char* arg;
		//! A temporary CString
		const char* sub;
		//! A temporary buffer
		char buffer[YUNI_GETOPT_LONGNAME_MAX_LENGTH + 1];

	}; // class Context






	inline Context::TokenType Context::GetTokenType(const char* argv)
	{
		# ifndef YUNI_OS_MSVC
		return (*argv == '-')
			? ((argv[1] == '-') ? ttLongOption : ttShortOption)
			: ttParameter;
		# else
		return (*argv == '-' || *argv == '/')
			? ((argv[1] == '-') ? ttLongOption : ttShortOption)
			: ttParameter;
		# endif
	}


	inline Context::Context(Parser& parser)
		:pParser(parser), pTokenIndex(1), pParameterIndex(1)
	{
		pParser.pErrors = 0;
	}


	bool Context::findNextParameter(IOption* option, int argc, char* argv[])
	{
		if (!option->requireAdditionalParameter())
		{
			option->enableFlag();
			return true;
		}
		while ((++pParameterIndex) < argc)
		{
			// We only want parameters
			if (ttParameter == GetTokenType(argv[pParameterIndex]))
			{
				// Adding the argument to the list of value of the option
				option->addValue(argv[pParameterIndex], static_cast<String::size_type>(::strlen(argv[pParameterIndex])));
				// This argument must not be used again as a parameter
				++pParameterIndex;
				return true;
			}
		}
		// If not found, it is an error
		++pParser.pErrors;
		return false;
	}


	void Context::optionIsUnknown(const char* name)
	{
		++pParser.pErrors;
		STD_CERR << "Error: The option `" << name << "` is unknown" << std::endl;
	}


	void Context::parameterIsMissing(const char* name)
	{
		++pParser.pErrors;
		STD_CERR << "Error: The parameter for `" << name << "` is missing" << std::endl;
	}


	bool Context::operator () (int argc, char* argv[])
	{
		while (pTokenIndex < argc)
		{
			arg = argv[pTokenIndex];
			switch (GetTokenType(arg))
			{
				// The current argument seems to be a short name of an option
				case ttShortOption:
					{
						while ('\0' != *(++arg))
						{
							OptionsOrderedByShortName::iterator i = pParser.pShortNames.find(*arg);
							if (i != pParser.pShortNames.end())
							{
								if (!findNextParameter(i->second, argc, argv))
									std::cerr << "Error: The parameter is missing for `" << arg << "`" << std::endl;
							}
							else
							{
								if (*arg == 'h' || *arg == '?')
								{
									pParser.helpUsage(argv[0]);
									return false;
								}
								optionIsUnknown(arg);
							}
						}
						break;
					}

				// The current argument seems to be a long name of an option
				case ttLongOption:
					{
						++arg;
						++arg;
						if ('\0' == *arg) // End of options
							return (!pParser.pErrors);
						if ((sub = strchr(arg, '=')))
						{
							const size_t size = static_cast<size_t>(sub - arg);
							if (size < sizeof(buffer))
							{
								# ifdef YUNI_OS_MSVC
								strncpy_s(buffer, sizeof(buffer), arg, size);
								# else
								strncpy(buffer, arg, size);
								# endif
								buffer[size] = '\0';
								arg += size;
								++arg;

								OptionsOrderedByLongName::iterator i = pParser.pLongNames.find(buffer);
								if (i != pParser.pLongNames.end())
									i->second->addValue(arg, static_cast<String::size_type>(::strlen(arg)));
								else
								{
									if (!::strcmp(buffer, "help"))
									{
										pParser.helpUsage(argv[0]);
										return false;
									}
									optionIsUnknown(buffer);
								}
							}
							else
							{
								std::cerr << "Error: name too long" << std::endl;
								++pParser.pErrors;
							}
						}
						else
						{
							OptionsOrderedByLongName::iterator i = pParser.pLongNames.find(arg);
							if (i != pParser.pLongNames.end())
							{
								if (!findNextParameter(i->second, argc, argv))
									parameterIsMissing(arg);
							}
							else
							{
								if (!::strcmp(arg, "help"))
								{
									pParser.helpUsage(argv[0]);
									return false;
								}
								optionIsUnknown(arg);
							}
						}
						break;
					}

					// The current argument is a parameter actually (not attached to any option)
				case ttParameter:
					{
						if (pTokenIndex >= pParameterIndex)
						{
							pParameterIndex = pTokenIndex;
							if (pParser.pRemains)
								pParser.pRemains->addValue(arg, static_cast<String::size_type>(::strlen(arg)));
						}
						break;
					}
			}
			++pTokenIndex;
		}
		return (!pParser.pErrors);
	}



} // namespace GetOptImpl
} // namespace Private
} // namespace Yuni








namespace // anonymous
{

	const char* ExtractFilenameOnly(const char* argv)
	{
		const char* result = argv;
		while ('\0' != *argv)
		{
			if ('\\' == *argv || '/' == *argv)
			{
				result = argv;
				++result;
			}
			++argv;
		}
		return result;
	}


} // anonymous namespace



namespace Yuni
{
namespace GetOpt
{

	Parser::Parser() :
		pRemains(NULL),
		pErrors(0)
	{}



	Parser::~Parser()
	{
		delete pRemains;

		if (!pAllOptions.empty())
		{
            // We should clear those maps before `pAllOptions` since they used references
            // from it
            pShortNames.clear();
            pLongNames.clear();

			OptionList::iterator end = pAllOptions.end();
			for (OptionList::iterator i = pAllOptions.begin(); i != end; ++i)
				delete *i;
		}
	}

	void Parser::clear()
	{
		if (!pAllOptions.empty())
		{
			pShortNames.clear();
			pLongNames.clear();

			OptionList::iterator end = pAllOptions.end();
			for (OptionList::iterator i = pAllOptions.begin(); i != end; ++i)
				delete *i;
			pAllOptions.clear();
		}
		delete pRemains;
		pRemains = NULL;
	}


	bool Parser::operator () (int argc, char* argv[])
	{
		Private::GetOptImpl::Context context(*this);
		return context(argc, argv);
	}


	void Parser::helpUsage(const char* argv0)
	{
		std::cout << "Usage: " << ExtractFilenameOnly(argv0) << " [OPTION]...";
		if (pRemains)
			std::cout << " [FILE]...\n";
		else
			std::cout << "\n";

		if (!pAllOptions.empty())
		{
			OptionList::const_iterator end = pAllOptions.end();
			OptionList::const_iterator i = pAllOptions.begin();

			// Add a space if the first option is not a paragraph
			// In this case the user would do what he wants
			if (!dynamic_cast<const Private::GetOptImpl::Paragraph*>(*i))
				std::cout << "\n";

			for (; i != end; ++i)
				(*i)->helpUsage(std::cout);
		}

		// Help
		if (pLongNames.end() == pLongNames.find("help"))
		{
			if (pShortNames.end() == pShortNames.find('h'))
				Private::GetOptImpl::DisplayHelpForOption(std::cout, 'h', "help", "Display this help and exit");
			else
				Private::GetOptImpl::DisplayHelpForOption(std::cout, ' ', "help", "Display this help and exit");
		}

		std::cout << "\n";
	}



	void Parser::appendShortOption(IOption* option, char shortname)
	{
		// In the list with all other options
		pAllOptions.push_back(option);
		// The short name
		if (shortname != ' ' && shortname != '\0')
			pShortNames[shortname] = option;
	}


	void Parser::appendOption(IOption* option, char shortname)
	{
		const String& longname = option->longName();
		if (!longname.empty())
		{
			// The long name of an option must not be equal to 1
			// There is an ambiguity on Windows : /s : a long or short name ?
			# ifndef NDEBUG
			assert(longname.size() != 1 && "The long name of an option must be igreater than 1 (ambigous on Windows)");
			# else
			if (longname.size() == 1) // ambigous on Windows, must not continue
				return;
			# endif

			pLongNames[longname.c_str()] = option;
		}

		// In the list with all other options
		pAllOptions.push_back(option);

		// The short name
		if (shortname != ' ' && shortname != '\0')
			pShortNames[shortname] = option;
	}




} // namespace GetOpt
} // namespace Yuni
