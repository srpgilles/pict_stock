#ifndef PHOTOGRAPHER_HXX_
# define PHOTOGRAPHER_HXX_

namespace PictStock
{
namespace ExtendedPhoto
{
namespace Private
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


	inline const TablePhotographers::FirstName::StringType& Photographer::firstName() const
	{
		return pFirstName;
	}

	inline const TablePhotographers::LastName::StringType& Photographer::lastName() const
	{
		return pLastName;
	}

	inline const TablePhotographers::Abbreviation::StringType&	Photographer::abbreviation() const
	{
		return pAbbreviation;
	}


} // namespace Private
} // namespace ExtendedPhoto
} // namespace PictStock

#endif /* PHOTOGRAPHER_HXX_ */
