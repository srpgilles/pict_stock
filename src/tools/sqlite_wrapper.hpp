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
		operator sqlite3_stmt*() { return pStatement; }

		//! Get the address of the underlying statement pointer
		sqlite3_stmt** ptr() { return &pStatement; }

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
		void createTable(const AnyString& name, const AnyString& fields, BehaviourWhenTableExists mode);



	private:

		//! Internal handler to sqlite3 database
		sqlite3* pDb;

	};

}

#endif /* SQLITE_WRAPPER_HPP_ */
