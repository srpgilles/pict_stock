/*
 * sqlite_wrapper.hxx
 *
 *  Created on: Oct 12, 2012
 *      Author: sebastien
 */

#ifndef SQLITE_WRAPPER_HXX_
# define SQLITE_WRAPPER_HXX_

namespace GenericTools
{
	inline SqliteStatement::operator sqlite3_stmt*()
	{
		return pStatement;
	}


	inline sqlite3_stmt** SqliteStatement::ptr()
	{
		return &pStatement;
	}


	inline int SqliteWrapper::prepareCommand(SqliteStatement& statement, const YString& command)
	{
		// size + 1 because terminal character must be counted (see sqlite website)
		return sqlite3_prepare_v2(pDb, command.c_str(), command.size() + 1, statement.ptr(), NULL);
	}


} // namespace GenericTools


#endif /* SQLITE_WRAPPER_HXX_ */
