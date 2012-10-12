#ifndef SQLITE_WRAPPER_HPP_
# define SQLITE_WRAPPER_HPP_


# include <yuni/core/string.h>
# include <sqlite3.h>

namespace GenericTools
{

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

	private:

		//! Internal handler to sqlite3 database
		sqlite3* pDb;

		//! Object that handles the last statement
		sqlite3_stmt* pStatement;



	};

}

#endif /* SQLITE_WRAPPER_HPP_ */
