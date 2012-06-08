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
namespace Private
{
	using namespace Yuni;

	namespace
	{
		template<std::size_t I = 0>
		typename std::enable_if<I == std::tuple_size<TupleType>::value, void>::type
			interpretUserDefinedFormatHelper(
			std::array<unsigned int, std::tuple_size<TupleType>::value>& /*array*/,
			const YString& /*format*/)
		  { }


		template<std::size_t I = 0>
		typename std::enable_if<I < std::tuple_size<TupleType>::value, void>::type
		interpretUserDefinedFormatHelper(
			std::array<unsigned int, std::tuple_size<TupleType>::value>& array,
			const YString& format)
		  {
			typedef typename std::tuple_element<I, TupleType>::type type;

			unsigned int pos = format.find(type::Symbol());

			array[I] = pos; // npos possibly stored

			interpretUserDefinedFormatHelper<I + 1>(array, format);
		  }


		template<std::size_t I = 0>
		typename std::enable_if<I == std::tuple_size<TupleType>::value, void>::type
			onlyUsefulOnesHelper(
				PathInformations& /* usefulInformations */,
				const PathInformations& /* completeInformations */,
				const std::bitset<std::tuple_size<TupleType>::value>& /*doContains*/
				)
		  { }


		template<std::size_t I = 0>
		typename std::enable_if<I < std::tuple_size<TupleType>::value, void>::type
			onlyUsefulOnesHelper(
				PathInformations& usefulInformations,
				const PathInformations& completeInformations,
				const std::bitset<std::tuple_size<TupleType>::value>& doContains
				)
		{
			if (doContains.test(I))
				usefulInformations.setValue(I, completeInformations.value(I));

			onlyUsefulOnesHelper<I + 1>(usefulInformations, completeInformations, doContains);
		}


		/*!
		** \brief Recursion to replace symbols by appropriate regular expression
		**
		** \param[out] out Regex that is requested
		** \param[in, out] helper In the very first call, the user-define format.
		** Each recursion will replace the symbol considered by the proper regex
		** \param[in] doContains Tells whether the symbol is present or not. If not,
		** no need to bother with two calls to string::replace!
		*/
		template<std::size_t I = 0>
		typename std::enable_if<I == std::tuple_size<TupleType>::value, void>::type
			determineRegexHelper(
				regexNS::regex& out,
				YString& helper,
				const std::bitset<std::tuple_size<TupleType>::value>& /*doContains*/
				)
		{
			out = regexNS::regex(helper.c_str());
		}


		template<std::size_t I = 0>
		typename std::enable_if<I < std::tuple_size<TupleType>::value, void>::type
			determineRegexHelper(
				regexNS::regex& out,
				YString& helper,
				const std::bitset<std::tuple_size<TupleType>::value>& doContains
				)
		{
			if (doContains.test(I))
			{
				typedef typename std::tuple_element<I, TupleType>::type type;
				auto symbol = type::Symbol();
				auto regex = type::Regex();

				unsigned int pos = helper.find(symbol);
				assert(pos != String::npos && "If equal, doContains test is bullshit!");

				helper.replace(pos + 1u, symbol, regex);

				// Now replace the first expression with parenthesis (for regex match)
				String buf('(');
				buf << regex << ')';
				helper.replace(symbol, buf);
			}

			// Recursion
			determineRegexHelper<I + 1>(out, helper, doContains);
		}


	} // namespace anonymous



	void PathFormatHelper::interpretUserDefinedFormat()
	{
		assert(pOrdering.empty() && "This method should be called once during construction");

		// First, find whether symbols are present and store the position in an array.
		// If not found, store npos instead of position value
		// Use for that a recursive template call over all elements of TupleType
		std::array<unsigned int, std::tuple_size<TupleType>::value> positions;
		interpretUserDefinedFormatHelper<0>(positions, pFormat);

		// Prepare the output vector: use a temporary map to determine the order
		std::map<unsigned int, size_t> helper;

		for (size_t i = 0u, size = positions.size(); i < size; ++i)
		{
			if (positions[i] == String::npos)
			{
				pDoContains[i] = false;
				continue;
			}

			pDoContains[i] = true;
			helper[positions[i]] = i;
		}

		for (auto it = helper.cbegin(), end = helper.cend(); it != end; ++it)
			pOrdering.push_back(it->second);
	}





	PathFormatException::PathFormatException(const AnyString& msg)
		: pMessage(msg)
	{ }


	PathFormatException::~PathFormatException() throw()
	{ }


	const char* PathFormatException::what() const throw()
	{
		return pMessage.c_str();
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
		assert(path.notEmpty());

		{
			// First determine the ordering of the symbols found in user-defined expression
			interpretUserDefinedFormat();
		}

		{
			// Determine regex expression by replacing symbols by adequate expression
			// If one is present more than once, only the first one is enclosed within parenthesis
			// (due to the way regex_results works)
			String helper(path);
			determineRegexHelper<0>(pRegEx, helper, pDoContains);
		}
	}


	bool PathFormatHelper::isOk(const AnyString& path,
		PathInformations& out) const
	{
		assert(out.isEmpty() && "If not, improper call to this method");
		regexNS::cmatch m;

		if (!regex_search(path.c_str(), m, pRegEx))
			return false;

		size_t size = pSymbolOrdering.size();

		for (unsigned int i = 0u; i < size; ++i)
		{
			auto ptr = pSymbolOrdering[i];
			assert(!(!ptr));

			out.setValue(ptr->nature, m[i + 1u].str());
			// + 1u because regex elements begin at 1; 0 is the full expression
		}

		return true;
	}


	void PathFormatHelper::determineMinimalPath(String& out,
		const PathInformations& infos) const
	{
		assert(out.empty());

		out = pFormat;

		for (auto it = pElements.cbegin(), end = pElements.cend(); it != end; ++it)
		{
			Traits::Element::Ptr elementPtr = *it;
			assert(!(!elementPtr));
			const Traits::Element& element = *elementPtr;

			if (doContains.test(element.nature))
				out.replace(element.symbol(), infos.value(element.nature));
		}
	}


	void PathFormatHelper::determineMinimalPath(Yuni::String& out,
		const ExtendedPhoto& photo) const
	{
		PathInformations::Ptr infosPtr = photo.informations();
		assert(!(!infosPtr));
		auto& infos = *infosPtr;
		determineMinimalPath(out, infos);
	}

	PathInformations PathInformations::onlyUsefulElements(
		const std::bitset<Elements::size>& arePresent) const
	{
		PathInformations ret(logs);

		for (unsigned int i = 0u; i < Elements::size; ++i)
		{
			if (arePresent.test(i))
				ret.setValue(i, value(i));
		}

		return ret;
	}


	PathInformations PathFormatHelper::onlyUsefulElements(const PathInformations& input) const
	{
		PathInformations ret(logs);
		onlyUsefulOnesHelper<0>(ret, input, pDoContains));
	}



} // namespace Private
} // namespace PictStock
