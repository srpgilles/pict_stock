# include <QApplication>
# include <QWidget>
# include <QStatusBar>
# include <QTabWidget>
# include <QHBoxLayout>
# include <QMenu>
# include <QMenuBar>
# include <cassert>

#include "main_window.hpp"
#include "tab_pict_frame.hpp"
#include "tab_sort_photos.hpp"
#include "tab_change_date.hpp"
#include "tab_photographers_cameras.hpp"


namespace PictStock
{
namespace Gui
{

    MainWindow::MainWindow()
        : QMainWindow(),
          pDb(nullptr),
          pCentralArea(nullptr),
          pStatusBar(nullptr),
          pTabLayout(nullptr),
          pTabManager(nullptr),
          pTabPictFrame(nullptr),
          pTabSortPhotos(nullptr),
          pTabPhotographersCameras(nullptr),
          pTabChangeDate(nullptr),
          pDlgNoDefaultDb(nullptr)
    {
        resize(640, 480);       

        pCentralArea = new QWidget(this);
        pStatusBar = statusBar();

        createMenuBar();
        createTabManager();
        setCentralWidget(pCentralArea);
        loadDatabaseWhenStarting();
    }


    void MainWindow::createMenuBar()
    {
        QMenuBar* menuBarPtr = menuBar();
        QMenuBar& menuBar = *menuBarPtr;

        menuDatabase(menuBar);
        menuQuit(menuBar);
    }


    void MainWindow::menuDatabase(QMenuBar &menuBar)
    {
        // Operations upon database (save a copy, change)
        QMenu* databaseMenu = menuBar.addMenu(tr("Database"));

        QAction* loadAction = new QAction(tr("Load new database"), this);
        QAction* saveCopyAction = new QAction(tr("Save copy of the database"), this);
        databaseMenu->addAction(loadAction);
        databaseMenu->addAction(saveCopyAction);
    }


    void MainWindow::menuQuit(QMenuBar &menuBar)
    {
        QMenu* quitMenu = menuBar.addMenu(tr("Quit"));

        QAction* quitAction = new QAction(tr("Quit"), this);
        quitMenu->addAction(quitAction);
        connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    }


    void MainWindow::createTabManager()
    {
        pTabLayout = new QHBoxLayout;
        pTabManager = new QTabWidget(pCentralArea);
        pTabSortPhotos = new TabSortPhotos;
        pTabPictFrame = new TabPictFrame;
        pTabChangeDate = new TabChangeDate;
        pTabPhotographersCameras = new TabPhotographersCameras;

        {
            assert(pTabManager);
            QTabWidget& tabManager = *pTabManager;
            assert(pTabSortPhotos && pTabPhotographersCameras
                && pTabPictFrame && pTabChangeDate);

            tabManager.addTab(pTabSortPhotos, tr("Sort new photos"));
            tabManager.addTab(pTabPhotographersCameras, tr("Photographers && cameras"));
            tabManager.addTab(pTabPictFrame, tr("Random selection"));
            tabManager.addTab(pTabChangeDate, tr("Change date"));
        }

        pTabLayout->addWidget(pTabManager);
        pCentralArea->setLayout(pTabLayout);
    }



} // namespace Gui
} // namespace PictStock


