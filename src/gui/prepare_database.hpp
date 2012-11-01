#ifndef __PICT_STOCK__GUI__PREPARE_DATABASE_HPP_
# define __PICT_STOCK__GUI__PREPARE_DATABASE_HPP_

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

    /*!
     * \brief The PrepareDatabase class is in charge of database operations of PictStock
     *
     * When a database is to be loaded, created, saved from a request from the GUI,
     * #PrepareDatabase is in charge of doing so.
     *
     * The idea is that the operation is attempted here, and in case of success the result
     * is transferred to MainWindow class (commit-or-rollback strategy)
     *
     * For instance, if another database is to be loaded, it is attempted in #PrepareDatabase.
     * If successful, the  #pDb object in MainWindow will be modified, if not the previous one
     * will be kept, thus ensuring there is always a database loaded.
     */
    class YUNI_DECL PrepareDatabase : public ::QWidget
    {
        Q_OBJECT

    public:

        //! Constructors
        //@{
        //! Essentially does nothing; Init() must be called
        explicit PrepareDatabase();

        //@}

        //! Performs the determination of db object at construct
        void AtConstruct();

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

        //! Close the dialog box and set the pointer to nullptr
        void closeDialogBox();

    private:

        //! QDialog object used when default database is not quite enough
        QDialog* pDialog;

    };


} // namespace Private
} // namespace Gui
} // namespace PictStock


#endif /* __PICT_STOCK__GUI__PREPARE_DATABASE_HPP_ */
