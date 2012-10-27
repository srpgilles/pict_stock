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

    namespace Private
    {
        class PrepareDatabase;
    }

    class MainWindow : public ::QMainWindow
    {
        Q_OBJECT

    public:

        //! Constructors
        //@{
        //
        MainWindow();

        //@}

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

    private slots:

        //! Init pDb
        void initDatabase(Database::Database* db);

        //! Save a copy of the database
        void saveCopyDatabase();




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

        //! Object in charge of preparing database
        Private::PrepareDatabase* pPrepDatabase;
    };


} // namespace Gui
} // namespace PictStock

#endif // _MAIN_WINDOW_HPP
