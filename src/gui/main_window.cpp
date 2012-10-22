# include <QWidget>
# include <QStatusBar>
# include <QTabWidget>
# include <QHBoxLayout>
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
          pCentralArea(nullptr),
          pStatusBar(nullptr),
          pTabLayout(nullptr),
          pTabManager(nullptr),
          pTabPictFrame(nullptr),
          pTabSortPhotos(nullptr),
          pTabPhotographersCameras(nullptr),
          pTabChangeDate(nullptr)
    {
        resize(640, 480);

        pCentralArea = new QWidget(this);
        pStatusBar = statusBar();

        createTabManager(); // to call before setCentralWidget()
        setCentralWidget(pCentralArea);
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


