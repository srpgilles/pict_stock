#ifndef __PICT_STOCK__TOOLS__SQLITE_WRAPPER_HPP_
# define __PICT_STOCK__TOOLS__SQLITE_WRAPPER_HPP_


# include <yuni/core/string.h>
# include <sqlite3.h>
# include <unordered_set>
# include "stl.hpp"
# include "exceptions.hpp"
# include "tuple_utilities.hpp"
# include "exceptions/sqlite_wrapper_exceptions.hpp"

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

	namespace PragmaForeignKeys // waiting for enum class support in MSVC
	{
		enum Values
		{
			off = 0,
			on = 1
		};
	}

	namespace PragmaSynchronous // waiting for enum class support in MSVC
	{
		enum Values
		{
			off = 0,
			normal = 1,
			full = 2
		};
	}

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
		 * \param[in] synchronous Off, normal or full (see sqlite documentation). Disabled by default
		 * (it is a huge performance hit, but we clearly loose safety here)
		 * \param[in] foreign_keys Enables or not foreign keys (enabled by default)
		 *
		 */
		explicit SqliteWrapper(const AnyString& dbName, int flags,
			PragmaSynchronous::Values synchronous = PragmaSynchronous::off,
			PragmaForeignKeys::Values foreignKey = PragmaForeignKeys::on);

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
		** \param[in] sqlQuery Without the "SELECT" word
		*/
		template<typename TupleT>
		void select(std::vector<TupleT>& out, const AnyString& sqlQuery) const;


		/*!
		** \brief Choose the status for foreign_keys
		**
		** See sqlite documentation for the whereabouts of this choice
		*/
		inline void pragmaForeignKeys(PragmaForeignKeys::Values choice);


		/*!
		** \brief Choose the status for synchronous
		**
		** See sqlite documentation for the whereabouts of this choice
		**
		** To put in a nutshell: full is much more secure but is a huge performance hit
		*/
		inline void pragmaSynchronous(PragmaSynchronous::Values choice);


        /*!
        ** \brief Get all sql schemas as an unordered set
        **
        ** The purpose is to be able to check with this data if a given file complies
        ** with the expected format of a sqlite database
        */
        void schemas(std::unordered_set<YString>& out) const;


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

#endif /* __PICT_STOCK__TOOLS__SQLITE_WRAPPER_HPP_ */
