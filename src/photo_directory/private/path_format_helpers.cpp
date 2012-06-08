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

			if (pos != String::pos)

			interpretUserDefinedFormatHelper<I + 1, TupleType>(out, format);
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


	const Traits::Element::Vector PathFormatHelper::pElements =
		{
			new Traits::Year(),
			new Traits::Month(),
			new Traits::Day(),
			new Traits::Hour(),
			new Traits::Minute(),
			new Traits::Second(),
			new Traits::Photographer()
		};




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
		assert(doContains.size() == pElements.size());

		{
			// First determine the ordering of the symbols found in user-defined expression
			typedef std::map<unsigned int, Traits::Element::Ptr> Position_Symbol;
			Position_Symbol positions;

			for (size_t i = 0u, size = pElements.size(); i < size; ++i)
			{
				auto elementPtr = pElements[i];
				assert(!(!elementPtr));
				auto& element = *elementPtr;
				auto pos = path.indexOf(0, element.symbol());

				if (pos != String::npos)
				{
					positions[pos] = elementPtr;
					doContains[element.nature] = true;
				}
				// no else: by default a bitset is filled with false everywhere
			}

			assert(pSymbolOrdering.empty() && "Method should only be called once in construction");
			pSymbolOrdering.reserve(positions.size());

			for (auto it = positions.cbegin(), end = positions.cend(); it != end; ++it)
			{
				assert(!(!(it->second)));
				pSymbolOrdering.push_back(it->second);
			}
		}

		{
			// Determine regex expression by replacing symbols by adequate expression
			// If one is present more than once, only the first one is enclosed within parenthesis
			// (due to the way regex_results works)

			String helper(path);

			for (size_t i = 1u, size = pSymbolOrdering.size(); i < size; ++i)
			{
				// We must use a trick as some useful string methods don't exist
				// (count the number of occurrence of a given substring, or replace only
				// first occurrence)
				auto elementPtr = pSymbolOrdering[i];
				assert(!(!elementPtr));
				auto& element = *elementPtr;
				auto symbol = element.symbol();
				auto regex = element.regex();
				unsigned int pos = helper.find(symbol);
				assert(pos != String::npos && "If equal, something went wrong above");

				helper.replace(pos + 1u, symbol, regex);

				// Now replace the first expression
				String buf('(');
				buf << regex << ')';
				helper.replace(symbol, buf);
			}

			pRegEx = regexNS::regex(helper.c_str());
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
