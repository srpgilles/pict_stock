/*
 * sqlite_wrapper.hxx
 *
 *  Created on: Oct 12, 2012
 *      Author: sebastien
 */

#ifndef SQLITE_WRAPPER_HXX_
# define SQLITE_WRAPPER_HXX_

# include "../tools/tools.hpp"
# include <tuple>

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


	inline int SqliteWrapper::prepareCommand(SqliteStatement& statement, const AnyString& command) const
	{
		// size + 1 because terminal character must be counted (see sqlite website)
		return sqlite3_prepare_v2(pDb, command.c_str(), command.size() + 1, statement.ptr(), NULL);
	}


	template<class TupleT>
	void SqliteWrapper::insertData(const AnyString& tableName, const std::vector<YString>& fields,
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
		if (errCode != SQLITE_OK)
		{
			YString message("Following insertion command is somehow incorrect:\n\t");
			message << command;
			throw Exception(message);
		}

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


	namespace Private
	{
		template<size_t Index, size_t Max, class TupleT>
		struct TupleElement
		{
			static void fill(SqliteStatement& statement, TupleT& tuple)
			{
				YString buf(reinterpret_cast<const char*>(sqlite3_column_text(statement, Index)));

				typedef typename std::tuple_element<Index, TupleT>::type Type;
				std::get<Index>(tuple) = buf.to<Type>();
				TupleElement<Index + 1, Max, TupleT>::fill(statement, tuple);
			};
		};


		template<size_t Max, class TupleT>
		struct TupleElement<Max, Max, TupleT>
		{
			static void fill(SqliteStatement&, TupleT& )
			{ };
		};
	}

	template<class TupleT>
	void SqliteWrapper::select(std::vector<TupleT>& out, const AnyString& sqlQuery) const
	{
		YString command("SELECT ");
		command << sqlQuery << ';';

		SqliteStatement statement;

		int errCode = prepareCommand(statement, command);
		assert(errCode == SQLITE_OK);

		assert(std::tuple_size<TupleT>::value == static_cast<size_t>(sqlite3_column_count(statement)));

		while ((errCode = sqlite3_step(statement)) && errCode == SQLITE_ROW)
		{
			// Read all the columns and fill a tuple accordingly
			TupleT tuple;
			Private::TupleElement<0, std::tuple_size<TupleT>::value, TupleT>::fill(statement, tuple);
			out.push_back(tuple);
		}

		if (errCode != SQLITE_DONE)
		{
			YString message("A problem occurred with the request:\n\t");
			message << command;
			throw Exception(message);
		}
	}


	inline void SqliteWrapper::pragmaForeignKeys(PragmaForeignKeys::Values choice)
	{
		Yuni::CString<30, false> cmd("PRAGMA foreign_keys = ");
		simpleCommand(cmd << static_cast<int>(choice));
	}

	inline void SqliteWrapper::pragmaSynchronous(PragmaSynchronous::Values choice)
	{
		Yuni::CString<30, false> cmd("PRAGMA synchronous = ");
		simpleCommand(cmd << static_cast<int>(choice));
	}

} // namespace GenericTools


#endif /* SQLITE_WRAPPER_HXX_ */
