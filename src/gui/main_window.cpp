#include "main_window.hpp"
#include "tab_pict_frame.hpp"
#include "tab_sort_photos.hpp"


#include <QHBoxLayout>


namespace PictStock
{
namespace Gui
{

    MainWindow::MainWindow()
        : QMainWindow()
    {
        resize(600, 450);
        pCentralArea = new QWidget(this);
        pStatusBar = statusBar();

        QHBoxLayout* tabLayout = new QHBoxLayout;
        pTabManager = new QTabWidget(pCentralArea);
        pTabSortPhotos = new TabSortPhotos;
        pTabPictFrame = new TabPictFrame;
        pTabManager->addTab(pTabSortPhotos, tr("Sort new photos"));
        pTabManager->addTab(pTabPictFrame, tr("Random selection"));
        tabLayout->addWidget(pTabManager);
        pCentralArea->setLayout(tabLayout);


        setCentralWidget(pCentralArea);

    }


} // namespace Gui
} // namespace PictStock


