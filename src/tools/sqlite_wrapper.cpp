#include "sqlite_wrapper.hpp"
#include "exceptions.hpp"

namespace GenericTools
{


	SqliteWrapper::SqliteWrapper(const AnyString& dbName, int flags)
		: pDb(nullptr),
		  pStatement(nullptr)
	{
		int errCode = sqlite3_open_v2(dbName.c_str(), &pDb, flags, NULL);

		if (errCode != SQLITE_OK)
			throw Exception(sqlite3_errmsg(pDb));
	}


	SqliteWrapper::~SqliteWrapper()
	{
		// Finalize the statement
		int errCode = sqlite3_finalize(pStatement);
		(void) errCode; // avoid warning in release mode compilation
		assert(errCode == SQLITE_OK);

		// Close the database connection
		errCode = sqlite3_close(pDb);
		assert(errCode == SQLITE_OK);
	}


}

