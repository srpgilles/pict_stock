#include <QApplication>
#include "gui/main_window.hpp"


using namespace PictStock;
using namespace PictStock::Gui;


int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();



    return app.exec();
}
