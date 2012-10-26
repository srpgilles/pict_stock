#ifndef _MAIN_WINDOW_HPP
# define _MAIN_WINDOW_HPP

# include <QMainWindow>
# include <memory>

# include "../database/database.hpp"
# include <yuni/core/string.h>

class QHBoxLayout;
class QWidget;
class QStatusBar;
class QMenuBar;
class QDialog;


namespace PictStock
{
namespace Gui
{
    class TabPictFrame;
    class TabSortPhotos;
    class TabPhotographersCameras;
    class TabChangeDate;

    class MainWindow : public ::QMainWindow
    {
         Q_OBJECT

    public:

        //! Constructors
        //@{
        //
        MainWindow();

        //@}

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

        /*!
        ** \brief Load the sqlite database when starting the application
        **
        ** If existing, load the default one, otherwise prompt the user
        ** to ask whether a default one should be created or to make him
        ** give the path to a valid one
        **/
        void loadDatabaseWhenStarting();

        /*!
        ** \brief Creates the tab manager
        */
        void createTabManager();

        /**
        ** \brief Create the menu bar
        */
        void createMenuBar();

        /**
        ** \brief Create the menu related to database operations
        */
        void menuDatabase(QMenuBar& menuBar);


        /**
        ** \brief Create the menu related to quit action
        */
        void menuQuit(QMenuBar& menuBar);

    private:



    private:

        //! Sqlite access
        std::unique_ptr<Database::Database> pDb;

        //! Central area
        QWidget* pCentralArea;

        //! Status bar
        QStatusBar* pStatusBar;

        //! Tab layout
        QHBoxLayout* pTabLayout;

        //! Tab manager
        QTabWidget* pTabManager;

        //! Tab related to PictFrame
        TabPictFrame* pTabPictFrame;

        //! Tab related to SortPhotos
        TabSortPhotos* pTabSortPhotos;

        //! Tab related to Photographers and cameras
        TabPhotographersCameras* pTabPhotographersCameras;

        //! Tab related to date modification
        TabChangeDate* pTabChangeDate;

        //! Dialog when no default database found when starting
        QDialog* pDlgNoDefaultDb;

    };


} // namespace Gui
} // namespace PictStock

#endif // _MAIN_WINDOW_HPP
