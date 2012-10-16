#include "cameras.hpp"
#include "../tools/sqlite_wrapper.hpp"

namespace PictStock
{

	Cameras::Cameras(GenericTools::SqliteWrapper& database)
	{
		database.select(pRows, "Keyword,Value,Owner FROM Cameras ORDER BY Keyword");
	}





} // namespace PictStock
