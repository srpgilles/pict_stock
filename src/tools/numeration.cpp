#include "numeration.hpp"
#include <yuni/core/math/math.h>
#include "exceptions.hpp"

namespace GenericTools
{
	namespace
	{
		// A function to determine the number of digits of an unsigned int
		unsigned int numberOfDigits(unsigned int number)
		{
			return static_cast<unsigned int>(Yuni::Math::Ceil(log10(static_cast<double>(number + 1u))));
		}
	} // namespace anonymous


	Numeration::Numeration(unsigned int nbElementsToDisplay)
		: pIndex(0),
		  pSize(nbElementsToDisplay)
	{
		unsigned int nbDigits = numberOfDigits(nbElementsToDisplay);

		if (nbDigits > maximumStringLength)
		{
			YString msg("Numeration wasn't expected to be called for list of numbers going as far as "
				"10^");
			msg << static_cast<unsigned int>(maximumStringLength) + 1u
				<< "; please modify Numeration::maximumStringLength to accommodate your needs";
			throw GenericTools::Exception(msg);
		}

		pString.resize(nbDigits);
		pString.fill('0');
	}


	const Numeration::TinyString& Numeration::next()
	{
		if (++pIndex > pSize)
		{
			YString msg("Numeration::next() was called too many times: it was expected ");
			msg << pSize << " times only and the " << pIndex << "th call has just been issued";
			throw GenericTools::Exception(msg);
		}

		pStringWithoutPadding.clear() << pIndex;
		pString.overwriteRight(pStringWithoutPadding);
		return pString;
	}


} // namespace GenericTools
