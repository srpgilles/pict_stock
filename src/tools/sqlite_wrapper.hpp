#ifndef SQLITE_WRAPPER_HPP_
# define SQLITE_WRAPPER_HPP_


# include <yuni/core/string.h>
# include <sqlite3.h>
# include "exceptions.hpp"

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
		 * \param[in] fla#include "exceptions.hpp"gs Flags related to the mode. See http://www.sqlite.org/c3ref/open.html
		 *
		 */

		explicit SqliteWrapper(const AnyString& dbName, int flags);

		//! Destructor
		~SqliteWrapper();
		//@}


		/*!
		 * \brief Create a table
		 *#include "exceptions.hpp"
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
		 * \tparam TupleT Tuple describing all columns
		 *
		 * \param[in] tableName Name of the table
		 * \param[in] fields Fields to insert in the row
		 * \param[in] values Values to insert in the row
		 *
		 * NOTE: that's not very edgy on my current software, but it is possible to
		 * be more efficient when inserting several rows (it is possible with
		 * sqlite to avoid reset the statement instead of preparing again from scratch)
		 */
		template<typename TupleT>
		void insertData(const AnyString& tableName, const std::vector<YString>& fields,
			const TupleT& values);


		/*!
		** \brief Select data from a table
		**
		** \tparam TupleT Tuple describing all columns
		**
		** \param[out] out Output rows
		** \param[in] sqlQuery Without the "SELECT" word
		*/
		template<typename TupleT>
		void select(std::vector<TupleT>& out, const AnyString& sqlQuery) const;


	private:

		/*!
		** \brief A wrapper over sqlite3_prepare_v2
		*/
		int prepareCommand(SqliteStatement& statement, const AnyString& command) const;


		/*!
		** \brief Call this method to issue a single command, which returns SQLITE_DONE
		**
		** For instance, command = PRAGMA synchronous = OFF
		*/
		void simpleCommand(const AnyString& command);

	private:

		//! Internal handler to sqlite3 database
		sqlite3* pDb;

	};

}

# include "sqlite_wrapper.hxx"

#endif /* SQLITE_WRAPPER_HPP_ */
