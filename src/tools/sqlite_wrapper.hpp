#ifndef SQLITE_WRAPPER_HPP_
# define SQLITE_WRAPPER_HPP_


# include <yuni/core/string.h>
# include <sqlite3.h>

namespace GenericTools
{

	/*!
	** \brief RAII idiom for sqlite3_stmt*
	*/
	class YUNI_DECL SqliteStatement
	{
	public:

		//! Constructors
		//@{
		/*!
		 * \brief Constructor
		 *
		 * \param[in] dbName Name of the file holding the database
		 * \param[in] flags Flags related to the mode. See http://www.sqlite.org/c3ref/open.html
		 *
		 */

		explicit SqliteStatement();

		//! Destructor
		~SqliteStatement();
		//@}

		//! Get the underlying pointer
		inline operator sqlite3_stmt*();

		//! Get the address of the underlying statement pointer
		inline sqlite3_stmt** ptr();

	private:

		//! Sqlite statement object
		sqlite3_stmt* pStatement;
	};


	/*!
	** \brief A very very thin wrapper to sqlite
	**
	** For my purpose the use of sqlite is extremely limited, but
	** I'd rather do it properly and use the RAII idiom
	**
	** I could have used one of many preexisting wrappers, but the ones
	** I found were either only for Windows, too circumvoluted or
	** one was great but featured prominently Boost (and I want to limit
	** as much as possible introducing too many libraries)
	*/

	class YUNI_DECL SqliteWrapper
	{
	public:

		enum BehaviourWhenTableExists
		{
			skipCreation, // be sure in this case the table get the same columns!
			reCreate, // beware: all data in existing table will be lost!
			sendError
		};

		//! Constructors
		//@{
		/*!
		 * \brief Constructor
		 *
		 * \param[in] dbName Name of the file holding the database
		 * \param[in] flags Flags related to the mode. See http://www.sqlite.org/c3ref/open.html
		 *
		 */

		explicit SqliteWrapper(const AnyString& dbName, int flags);

		//! Destructor
		~SqliteWrapper();
		//@}


		/*!
		 * \brief Create a table
		 *
		 * \param[in] name Name of the table
		 * \param[in] fields List of fields as they would appear in standard SQL command,
		 * separated by commas
		 * For instance "FirstName varchar(80), LastName varchar(80), Abbr varchar(8)";
		 */
		void createTable(const AnyString& name, const AnyString& fields, BehaviourWhenTableExists mode = sendError);


		/*!
		 * \brief Check whether foreign keys are correctly ON
		 *
		 */
		bool checkForeignKeys() const;

		/*!
		 * \brief Insert data in a table
		 *
		 * \param[in] tableName Name of the table
		 * \param[in] fields Fields to insert in the row
		 * \param[in] values Values to insert in the row
		 */
		void insertData(const AnyString& tableName, const AnyString& fields,
			const AnyString& values);

	private:

		/*!
		** \brief A wrapper over sqlite3_prepare_v2
		*/
		int prepareCommand(SqliteStatement& statement, const YString& command) const;


	private:

		//! Internal handler to sqlite3 database
		sqlite3* pDb;

	};

}

# include "sqlite_wrapper.hxx"

#endif /* SQLITE_WRAPPER_HPP_ */
