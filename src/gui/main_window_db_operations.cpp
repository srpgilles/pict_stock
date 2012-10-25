# include <cassert>

#include "main_window.hpp"
#include "../tools/sqlite_wrapper.hpp"
#include <yuni/core/string.h>
#include <yuni/io/directory/system.h>
#include <yuni/io/file/file.h>

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

using namespace Yuni;

namespace PictStock
{
namespace Gui
{

    void MainWindow::loadDatabaseWhenStarting()
    {  
        // First check the default emplacement; if it exists load it
        YString userDirectory;

        if (IO::Directory::System::UserHome(userDirectory))
        {
            YString defaultDatabasePath(userDirectory);
            defaultDatabasePath << IO::Separator << ".pict_stock"
                                <<  IO::Separator << "pict_stock.db3";

            if (IO::File::Exists(defaultDatabasePath))
            {
                std::cout << "EXIST\n";

                // Load the file found
                if (loadSqliteFile(defaultDatabasePath))
                    return;
                // TODO Foresee procedure if sqlite file does not match expected format
            }
        }


        {
            // If default database doesn't exist, fire up a dialog box

            QDialog* dialog = new QDialog(this);
            dialog->setModal(true);

            QVBoxLayout* dialogLayout = new QVBoxLayout;

            QLabel* question = new QLabel(tr("No default database found; what do you want to do?"));
            dialogLayout->addWidget(question);

            QHBoxLayout* optionsLayout = new QHBoxLayout;
            QPushButton* createDefault = new QPushButton(tr("Create default one"));
            QPushButton* loadAnother = new QPushButton(tr("Load existing one"));
            optionsLayout->addWidget(createDefault);
            optionsLayout->addWidget(loadAnother);
            dialogLayout->addLayout(optionsLayout);

            dialog->setLayout(dialogLayout);
            dialog->show();
        }
    }


    bool MainWindow::loadSqliteFile(const YString& db3File)
    {
        assert(IO::File::Exists(db3File));

        std::unique_ptr<GenericTools::SqliteWrapper> ptr {
            new GenericTools::SqliteWrapper(db3File, SQLITE_OPEN_READWRITE)
        };

        // TODO Here check whether the sqlite3 file fulfills the requirements



        pDb = std::move(ptr);
    }


} // namespace Gui
} // namespace PictStock


