#ifndef PREPARE_DATABASE_HPP
# define PREPARE_DATABASE_HPP

# include <QWidget>
# include "../database/database.hpp"

class QDialog;
class QFileDialog;

namespace PictStock
{
namespace Gui
{
namespace Private
{

    class PrepareDatabase : public ::QWidget
    {
        Q_OBJECT

    public:

        //! Constructors
        //@{
        //! Essentially does nothing; Init() must be called
        explicit PrepareDatabase();

        //@}

        //! Performs the determination of db object
        void Init();

    signals:

        /*! Emit this signal when underlying database has been initialised */
        void databaseInitialised(Database::Database*);

    private slots:

        /*! Create a new database at the default location */
        void createDefaultDatabase();

        /*! Create a new database at user-defined location */
        void createNonDefaultDatabase();

        /*! Effectively create the database once a path has been selected */
        void createNonDefaultDatabaseHelper(const QString& file);

        /*! Load an existing database. Prompt a dialog box to choose it. */
        void loadDatabase();

    private:

        //! QDialog object used when default database is not quite enough
        QDialog* pDialog;

    };


} // namespace Private
} // namespace Gui
} // namespace PictStock


#endif // PREPAREDATABASE_HPP
