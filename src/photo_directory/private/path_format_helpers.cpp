#include "path_format_helpers.hpp"

#include "../../extended_photo/extended_photo.hpp"
#include <array>



# ifdef USE_BOOST_REGULAR_EXPR
namespace regexNS = boost;
# else
namespace regexNS = std;
# endif // USE_BOOST_REGULAR_EXPR

namespace PictStock
{
namespace PhotoDirectory
{
namespace Private
{
	using namespace Yuni;
	typedef PictStock::ExtendedPhoto::Private::TupleType TupleInfosType;

	namespace
	{
		template<std::size_t I>
		typename std::enable_if<I == std::tuple_size<TupleInfosType>::value, void>::type
			interpretUserDefinedFormatHelper(
			std::array<unsigned int, std::tuple_size<TupleInfosType>::value>& /*array*/,
			const YString& /*format*/)
		  { }


		template<std::size_t I>
		typename std::enable_if<I < std::tuple_size<TupleInfosType>::value, void>::type
		interpretUserDefinedFormatHelper(
			std::array<unsigned int, std::tuple_size<TupleInfosType>::value>& array,
			const YString& format)
		  {
			typedef typename std::tuple_element<I, TupleInfosType>::type type;

			unsigned int pos = format.find(type::Symbol());

			array[I] = pos; // npos possibly stored

			interpretUserDefinedFormatHelper<I + 1>(array, format);
		  }


		template<std::size_t I>
		typename std::enable_if<I == std::tuple_size<TupleInfosType>::value, void>::type
			onlyUsefulOnesHelper(
				ExtendedPhoto::PathInformations& /* usefulInformations */,
				const ExtendedPhoto::PathInformations& /* completeInformations */,
				const PathFormatHelper::MatchingType& /*matching*/
				)
		  { }


		template<std::size_t I>
		typename std::enable_if<I < std::tuple_size<TupleInfosType>::value, void>::type
			onlyUsefulOnesHelper(
				ExtendedPhoto::PathInformations& usefulInformations,
				const ExtendedPhoto::PathInformations& completeInformations,
				const PathFormatHelper::MatchingType& matching
				)
		{
			if (matching.find(I) != matching.end())
			{
				typedef typename std::tuple_element<I, TupleInfosType>::type type;
				usefulInformations.setElement<type>(completeInformations);
			}

			onlyUsefulOnesHelper<I + 1>(usefulInformations, completeInformations, matching);
		}


		/*!
		** \brief Recursion to replace symbols by appropriate regular expression
		**
		** \param[out] out Regex that is requested
		** \param[in, out] helper In the very first call, the user-define format.
		** Each recursion will replace the symbol considered by the proper regex
		** \param[in] matching PathFormatHelper::MatchingType
		*/
		template<std::size_t I>
		typename std::enable_if<I == std::tuple_size<TupleInfosType>::value, void>::type
			determineRegexHelper(
				regexNS::regex& out,
				YString& helper,
				const PathFormatHelper::MatchingType& /*matching*/
				)
		{
			out = regexNS::regex(helper.c_str());
		}


		template<std::size_t I>
		typename std::enable_if<I < std::tuple_size<TupleInfosType>::value, void>::type
			determineRegexHelper(
				regexNS::regex& out,
				YString& helper,
				const PathFormatHelper::MatchingType& matching
				)
		{
			if (matching.find(I) != matching.end())
			{
				typedef typename std::tuple_element<I, TupleInfosType>::type type;
				auto symbol = type::Symbol();
				auto regex = type::Regex();

				unsigned int pos = helper.find(symbol);
				assert(pos != String::npos && "If equal, matching test is bullshit!");

				helper.replace(pos + 1u, symbol, regex);

				// Now replace the first expression with parenthesis (for regex match)
				String buf('(');
				buf << regex << ')';
				helper.replace(symbol, buf);
			}

			// Recursion
			determineRegexHelper<I + 1>(out, helper, matching);
		}


		/*!
		** \brief Recursion to fill #PathInformations object from a path that happens to match
		** regular expression obtained from user-defined format
		**
		** \param[out] out PathInformations object properly filled
		** \param[in] match Object resulting from the regex match
		** \param[in] matchingIndex PathFormathelper::pMatching
		*/
		template<std::size_t I>
		typename std::enable_if<I == std::tuple_size<TupleInfosType>::value, void>::type
			isOkHelper(
				ExtendedPhoto::PathInformations& /*out*/,
				const regexNS::cmatch& /*match*/,
				const PathFormatHelper::MatchingType& /*matching*/)
		{ }



		template<std::size_t I>
		typename std::enable_if<I < std::tuple_size<TupleInfosType>::value, void>::type
			isOkHelper(
				ExtendedPhoto::PathInformations& out,
				const regexNS::cmatch& match,
				const PathFormatHelper::MatchingType& matching)
		{
			auto it = matching.find(I);
			if (it != matching.end())
			{
				typedef typename std::tuple_element<I, TupleInfosType>::type type;
				out.setElement<type>(match[static_cast<int>(it->second)].str());
			}

			isOkHelper<I + 1>(out, match, matching);
		}


		/*!
		** \brief Recursion to determine minimal path
		**
		** \param[out] out Path under treatment (in very last recursion all substitutions will be done)
		** \param[in] infos Informations which will replace symbols in format
		** \param[in] matching PathFormat::pMatching. Point is just to avoid useless calls to replace
		*/
		template<std::size_t I>
		typename std::enable_if<I == std::tuple_size<TupleInfosType>::value, void>::type
			determineMinimalPathHelper(
				YString& /*out*/,
				const ExtendedPhoto::PathInformations& /*infos*/,
				const PathFormatHelper::MatchingType& /*matching*/)
		{ }



		template<std::size_t I>
		typename std::enable_if<I < std::tuple_size<TupleInfosType>::value, void>::type
			determineMinimalPathHelper(
				YString& out,
				const ExtendedPhoto::PathInformations& infos,
				const PathFormatHelper::MatchingType& matching)
		{
			auto it = matching.find(I);

			if (it != matching.cend())
			{
				typedef typename std::tuple_element<I, TupleInfosType>::type type;
				auto symbol = type::Symbol();
				out.replace(type::Symbol(), infos.getElement<type>());
			}

			// Recursion
			determineMinimalPathHelper<I + 1>(out, infos, matching);
		}


	} // namespace anonymous



	void PathFormatHelper::interpretUserDefinedFormat()
	{
		assert(pMatching.empty() && "This method should be called once during construction");

		// First, find whether symbols are present and store the position in an array.
		// If not found, store npos instead of position value
		// Use for that a recursive template call over all elements of TupleInfosType
		std::array<unsigned int, std::tuple_size<TupleInfosType>::value> positions;
		interpretUserDefinedFormatHelper<0>(positions, pFormat);

		// Prepare the output vector: use a temporary map to determine the order
		std::map<unsigned int, unsigned int> helper;

		for (unsigned int i = 0u, size = static_cast<unsigned int>(positions.size()); i < size; ++i)
		{
			if (positions[i] == String::npos)
				continue;

			helper[positions[i]] = i;
		}

		unsigned int index = 0u;
		for (auto it = helper.cbegin(), end = helper.cend(); it != end; ++it)
			pMatching[it->second] = ++index;
	}


	PathFormatHelper::PathFormatHelper(LoggingFacility& logs, const AnyString& format)
		: logs(logs),
		  pFormat(format)
	{
		setRegEx();
	}


	void PathFormatHelper::setRegEx()
	{
		const auto& path = pFormat;
		assert(!path.empty());

		{
			// First determine the ordering of the symbols found in user-defined expression
			interpretUserDefinedFormat();
		}

		{
			// Determine regex expression by replacing symbols by adequate expression
			// If one is present more than once, only the first one is enclosed within parenthesis
			// (due to the way regex_results works)
			String helper(path);
			determineRegexHelper<0>(pRegEx, helper, pMatching);
		}
	}


	bool PathFormatHelper::isOk(const AnyString& path,
		ExtendedPhoto::PathInformations& out) const
	{
		assert(out.isEmpty() && "If not, improper call to this method");
		regexNS::cmatch m;

		if (!regex_search(path.c_str(), m, pRegEx))
			return false;

		// Recursion to get relevant informations
		isOkHelper<0>(out, m, pMatching);

		return true;
	}


	void PathFormatHelper::determineMinimalPath(String& out,
		const ExtendedPhoto::PathInformations& infos) const
	{
		assert(out.empty());
		out = pFormat;

		// Recursive call over all tuple elements
		determineMinimalPathHelper<0>(out, infos, pMatching);
	}


	void PathFormatHelper::determineMinimalPath(Yuni::String& out,
		const ExtendedPhoto::ExtendedPhoto& photo) const
	{
		auto infosPtr = photo.informations();
		assert(!(!infosPtr));
		auto& infos = *infosPtr;
		determineMinimalPath(out, infos);
	}


	void PathFormatHelper::onlyUsefulElements(ExtendedPhoto::PathInformations& out,
		const ExtendedPhoto::PathInformations& input) const
	{
		onlyUsefulOnesHelper<0>(out, input, pMatching);
	}


	void PathFormatHelper::onlyUsefulElements(ExtendedPhoto::PathInformations& out,
		const ExtendedPhoto::ExtendedPhoto& input) const
	{
		auto infosPtr = input.informations();
		assert(!(!infosPtr));
		onlyUsefulElements(out, *infosPtr);
	}


} // namespace Private
} // namespace PhotoDirectory
} // namespace PictStock
