# include <cassert>

#include "main_window.hpp"
#include "../tools/sqlite_wrapper.hpp"
#include <yuni/core/string.h>
#include <yuni/io/directory/system.h>
#include <yuni/io/file/file.h>
#include "../database/exceptions/database.hpp"

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


    bool MainWindow::defaultDb3Location(YString& out) const
    {
        out.clear();

        if (!IO::Directory::System::UserHome(out))
            return false;

        out <<  IO::Separator << ".pict_stock";

        // Create the directory in memory if it doesn't exist
        if (!IO::Directory::Exists(out))
        {
            if (!IO::Directory::Create(out))
                return false;
        }

        out <<  IO::Separator << "pict_stock.db3";
        return true;
    }



    void MainWindow::loadDatabaseWhenStarting()
    {  
        // First check the default emplacement; if it exists load it
        YString defaultDatabasePath;

        if (defaultDb3Location(defaultDatabasePath))
        {
            if (IO::File::Exists(defaultDatabasePath))
            {
                try
                {
                    std::unique_ptr<Database::Database> ptr(new Database::Database(defaultDatabasePath,
                        Database::nsTable::load));
                    pDb = std::move(ptr);
                    return;
                }
                catch(const Database::Exceptions::DatabaseException& e)
                {
                    // TODO Foresee the message to display in dialog box

                }
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

            connect(createDefault, SIGNAL(clicked()), this, SLOT(createDefaultDatabase()));
        }
    }


    void MainWindow::createDefaultDatabase()
    {
        YString defaultPath;
        if (!defaultDb3Location(defaultPath))
            return;

        std::unique_ptr<Database::Database> ptr(new Database::Database(defaultPath,
            Database::nsTable::createAndLoad));
        pDb = std::move(ptr);
    }


} // namespace Gui
} // namespace PictStock


