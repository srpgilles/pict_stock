#ifndef _MAIN_WINDOW_HPP
# define _MAIN_WINDOW_HPP

# include <QMainWindow>

class QHBoxLayout;
class QWidget;
class QStatusBar;
class QMenuBar;

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
    public:

        //! Constructors
        //@{
        //
        MainWindow();

        //@}

    private:

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

    };


} // namespace Gui
} // namespace PictStock

#endif // _MAIN_WINDOW_HPP
