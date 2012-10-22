#ifndef _MAIN_WINDOW_HPP
# define _MAIN_WINDOW_HPP

# include <QMainWindow>
# include <QWidget>
# include <QStatusBar>
# include <QTabWidget>


namespace PictStock
{
namespace Gui
{
    class TabPictFrame;
    class TabSortPhotos;


    class MainWindow : public QMainWindow
    {
    public:

        //! Constructors
        //@{
        //
        MainWindow();

        //@}

    private:

        //! Central area
        QWidget* pCentralArea;

        //! Status bar
        QStatusBar* pStatusBar;

        //! Tab manager
        QTabWidget* pTabManager;

        //! Tab related to PictFrame
        TabPictFrame* pTabPictFrame;

        //! Tab related to SortPhotos
        TabSortPhotos* pTabSortPhotos;

    };


} // namespace Gui
} // namespace PictStock

#endif // _MAIN_WINDOW_HPP
