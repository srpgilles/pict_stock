#include "sqlite_wrapper.hpp"
#include "exceptions.hpp"

namespace GenericTools
{

	SqliteStatement::SqliteStatement()
		: pStatement(nullptr)
	{ }

	SqliteStatement::~SqliteStatement()
	{
		int errCode = sqlite3_finalize(pStatement);
		(void) errCode;
		assert(errCode == SQLITE_OK);
	}

	SqliteWrapper::SqliteWrapper(const AnyString& dbName, int flags)
		: pDb(nullptr)
	{
		int errCode = sqlite3_open_v2(dbName.c_str(), &pDb, flags, NULL);

		if (errCode != SQLITE_OK)
		{
			YString message("Problem while opening database ");
			message << dbName  << ": " << sqlite3_errmsg(pDb);
			throw Exception(message);
		}

		{
			// Enables foreign keys
			YString command("PRAGMA foreign_keys = ON;");
			SqliteStatement statement;

			int errCode = prepareCommand(statement, command);
			assert(errCode == SQLITE_OK);
			errCode = sqlite3_step(statement);
			assert(errCode == SQLITE_DONE);
		}
	}


	SqliteWrapper::~SqliteWrapper()
	{
		// Close the database connection
		int errCode = sqlite3_close(pDb);
		(void) errCode;
		assert(errCode == SQLITE_OK);
	}


	void SqliteWrapper::createTable(const AnyString& name, const AnyString& fields, BehaviourWhenTableExists mode)
	{
		YString command("CREATE TABLE ");
		command << name << '(' << fields << ");";

		SqliteStatement statement;

		int errCode = prepareCommand(statement, command);

		if (errCode == SQLITE_OK)
		{
			errCode = sqlite3_step(statement);
			if (errCode != SQLITE_DONE)
			{
				YString message("Problem while creating new table ");
				message << name << ": " << sqlite3_errmsg(pDb);
				throw Exception(message);
			}
		}
		else
		{
			{
				// If preparation goes wrong, the most likely causes are:
				// - the table already exists
				// - the syntax of the request is somehow incorrect
				// Check the first point; if not throw directly an exception
				SqliteStatement checkTableExists;
				YString checkCommand("SELECT count(*) FROM ");
				checkCommand << name << ';';
				errCode = prepareCommand(checkTableExists, checkCommand);

				if (errCode != SQLITE_OK)
				{
					YString message("Problem while creating new table ");
					message << name << ": the syntax is most likely incorrect. It was:";
					message << "\n\t" << command;
					throw Exception(message);
				}
			}

			switch(mode)
			{
				case skipCreation:
					break;
				case reCreate:
				{
					YString dropCommand = "DROP TABLE ";
					dropCommand << name << ';';

					SqliteStatement dropStatement;

					errCode = prepareCommand(dropStatement, dropCommand);
					assert(errCode == SQLITE_OK);
					errCode = sqlite3_step(dropStatement);
					assert(errCode == SQLITE_DONE);


					// Call again current method but this time error won't be tolerated
					// (and shouldn't occur either as table has been alledgely dropped)
					createTable(name, fields, sendError);
					return;
					break;
				}
				case sendError:
				{
					std::cout << "Err code = " << errCode << '\n';
					YString msg("Impossible to create table ");
					msg << name << ": it probably already exists";
					throw Exception(msg);
					break;
				}
			} // switch(mode)
		}//errCode != SQLITE_OK)
	}


	void SqliteWrapper::insertData(const AnyString& tableName, const std::vector<AnyString>& fields,
		const std::vector<AnyString>& values)
	{
		size_t size = fields.size();
		if (size != values.size())
		{
			YString message("Problem while inserting new row in table ");
			message << tableName << ": number of fields differ from number of values.";
			throw Exception(message);
		}

		YString command("INSERT INTO ");
		command << tableName << '(';
		for (unsigned int i = 0u; i < size - 1u; ++i)
			command << '\"' << fields[i] << "\",";
		command << '\"' << fields.back() << '\"';
		command << ") VALUES (";
		for (unsigned int i = 0u; i < size - 1u; ++i)
			command << '\"' << values[i] << "\",";
		command << '\"' << values.back() << "\");";

		SqliteStatement statement;

		std::cout << command << '\n';

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


	bool SqliteWrapper::checkForeignKeys() const
	{
		YString command = "PRAGMA foreign_keys;";
		SqliteStatement statement;

		int errCode = prepareCommand(statement, command);
		assert(errCode == SQLITE_OK);
		errCode = sqlite3_step(statement);
		assert(errCode == SQLITE_ROW);
		return (1 == sqlite3_column_int(statement, 0));
	}


}

