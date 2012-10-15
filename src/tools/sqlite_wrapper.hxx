/*
 * sqlite_wrapper.hxx
 *
 *  Created on: Oct 12, 2012
 *      Author: sebastien
 */

#ifndef SQLITE_WRAPPER_HXX_
# define SQLITE_WRAPPER_HXX_

# include "../tools/tools.hpp"

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


	inline int SqliteWrapper::prepareCommand(SqliteStatement& statement, const YString& command) const
	{
		// size + 1 because terminal character must be counted (see sqlite website)
		return sqlite3_prepare_v2(pDb, command.c_str(), command.size() + 1, statement.ptr(), NULL);
	}


	template<class TupleT>
	void SqliteWrapper::insertData(const AnyString& tableName, const std::vector<AnyString>& fields,
		const TupleT& values)
	{
		if (fields.size() != std::tuple_size<TupleT>::value)
		{
			YString message("Problem while inserting new row in table ");
			message << tableName << ": number of fields (" << fields.size()
				<< ") differ from number of values (" << std::tuple_size<TupleT>::value << ").";
			throw Exception(message);
		}

		YString command("INSERT INTO ");
		command << tableName << '(';

		{
			std::for_each(fields.begin(), fields.end(), [&command](const AnyString& elt)
				{
					command << '"' << elt << "\",";
				});
			command.removeLast();
		}
		command << ") VALUES (";

		{
			printTuple<YString, AnyString>(command, values, ",", "", "");
		}
		command << ");";

		SqliteStatement statement;

		int errCode = prepareCommand(statement, command);
		assert(errCode == SQLITE_OK);

		errCode = sqlite3_step(statement);

		switch(errCode)
		{
			case SQLITE_DONE:
				break;
			case SQLITE_CONSTRAINT:
			{
				YString message("Problem while inserting new row in table ");
							message << tableName << ": a constraint has not been respected. Command was:";
							message << "\n\t" << command;
							throw Exception(message);
			}
			default:
			{
				YString message("Problem while inserting new row in table ");
				message << tableName << ": the syntax is most likely incorrect. It was:";
				message << "\n\t" << command;
				throw Exception(message);
			}
		}// switch
	}


} // namespace GenericTools


#endif /* SQLITE_WRAPPER_HXX_ */
