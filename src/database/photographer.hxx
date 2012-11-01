#ifndef __PICT_STOCK__DATABASE__PHOTOGRAPHER_HXX_
# define __PICT_STOCK__DATABASE__PHOTOGRAPHER_HXX_

namespace PictStock
{
namespace Database
{

	inline bool operator==(const Photographer& lhs, const Photographer& rhs)
	{
		// As Abbreviation acts as a primary key, this condition should be quite enough
		// If not, something very fishy happened elsewhere (big bad bug...)
		return (lhs.pAbbreviation == rhs.pAbbreviation);
	}


	inline bool operator!=(const Photographer& lhs, const Photographer& rhs)
	{
		return !(operator == (lhs, rhs));
	}


	inline const TablePhotographers::FirstName::WrappedType& Photographer::firstName() const
	{
		return pFirstName;
	}

	inline const TablePhotographers::LastName::WrappedType& Photographer::lastName() const
	{
		return pLastName;
	}

	inline const TablePhotographers::Abbreviation::WrappedType&	Photographer::abbreviation() const
	{
		return pAbbreviation;
	}


} // namespace Database
} // namespace PictStock

#endif /* __PICT_STOCK__DATABASE__PHOTOGRAPHER_HXX_ */
