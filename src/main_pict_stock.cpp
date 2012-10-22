#include <QApplication>
#include "gui/main_window.hpp"
#include <QTextCodec>


using namespace PictStock;
using namespace PictStock::Gui;


int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));


    MainWindow mainWindow;
    mainWindow.show();



    return app.exec();
}
