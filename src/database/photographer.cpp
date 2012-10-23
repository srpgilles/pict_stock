#include "../tools/tools.hpp"
#include "photographer.hpp"

namespace PictStock
{
namespace Database
{

	Photographer::Photographer(const TupleWrappedType& row)
		: pFirstName(std::get<GenericTools::IndexOf<TablePhotographers::FirstName, Tuple>::value>(row)),
		  pLastName(std::get<GenericTools::IndexOf<TablePhotographers::LastName, Tuple>::value>(row)),
		  pAbbreviation(std::get<GenericTools::IndexOf<TablePhotographers::Abbreviation, Tuple>::value>(row))
	{ }

} // namespace Database
} // namespace PictStock
