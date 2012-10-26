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
#include <QFileDialog>

using namespace Yuni;

namespace PictStock
{
namespace Gui
{

    namespace
    {
        bool defaultDb3Location(YString& out)
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

        namespace LoadDatabaseStatus
        {
            enum Values
            {
                ok,
                does_not_exist,
                exist_but_invalid,
                no_home_directory, // if it wasn't possible to find home directory
                undetermined // default value before any other could be determined
            };
        }


        /*!
        ** \brief Class in charge of the dialog box when defautl database couldn't be
        ** load (for any reason)
        */
        class DialogStartingDatabase : public QDialog
        {



        };


    } // namespace anonymous



    void MainWindow::loadDatabaseWhenStarting()
    {  
        // First check the default emplacement; if it exists load it
        YString defaultDatabasePath;

        LoadDatabaseStatus::Values status(LoadDatabaseStatus::undetermined);

        if (defaultDb3Location(defaultDatabasePath))
        {
            if (IO::File::Exists(defaultDatabasePath))
            {
                try
                {
                    std::unique_ptr<Database::Database> ptr(new Database::Database(defaultDatabasePath,
                        Database::nsTable::load));
                    pDb = std::move(ptr);
                    //status = LoadDatabaseStatus::ok; Useless at the moment, but to keep in mind
                    // if refactoring
                    return;
                }
                catch(const Database::Exceptions::DatabaseException& e)
                {
                   status = LoadDatabaseStatus::exist_but_invalid;
                }
            }
            else
                status = LoadDatabaseStatus::does_not_exist;
        }        


        {
            // If default database doesn't exist, fire up a dialog box
            pDlgNoDefaultDb = new QDialog(this);
            pDlgNoDefaultDb->setModal(true);

            QVBoxLayout* dialogLayout = new QVBoxLayout;


            QHBoxLayout* optionsLayout = new QHBoxLayout;
            QPushButton* createDefault = new QPushButton(tr("Create default one"));
            QPushButton* createNonDefault = new QPushButton(tr("Create user-determined one"));
            QPushButton* loadAnother = new QPushButton(tr("Load existing one"));

            QLabel* question;

            switch(status)
            {
            case LoadDatabaseStatus::does_not_exist:
                question = new QLabel(tr("No default database found; what do you want to do?"));
                optionsLayout->addWidget(createDefault);
                break;
            case LoadDatabaseStatus::exist_but_invalid:
                question = new QLabel(tr("Default database does exist but is not valid; what do you want to do?"));
                break;
            case LoadDatabaseStatus::no_home_directory:
                question = new QLabel(tr("Unable to determine home directory; what do you want to do?"));
                break;
            case LoadDatabaseStatus::ok:
            case LoadDatabaseStatus::undetermined:
                assert("Check the code; it shouldn't happen" && false);
            }


            dialogLayout->addWidget(question);
            optionsLayout->addWidget(createNonDefault);
            optionsLayout->addWidget(loadAnother);
            dialogLayout->addLayout(optionsLayout);

            pDlgNoDefaultDb->setLayout(dialogLayout);
            pDlgNoDefaultDb->show();

            connect(createDefault, SIGNAL(clicked()), this, SLOT(createDefaultDatabase()));
            connect(createDefault, SIGNAL(clicked()), pDlgNoDefaultDb, SLOT(accept()));

            connect(createNonDefault, SIGNAL(clicked()), this, SLOT(createNonDefaultDatabase()));


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


    void MainWindow::createNonDefaultDatabase()
    {
        QFileDialog* dialog = new QFileDialog(this, QString(), "", tr("Sqlite database file (*.db3)"));
        dialog->setModal(true);
        dialog->setDefaultSuffix("db3");
        dialog->setAcceptMode(QFileDialog::AcceptSave);
        dialog->show();

        connect(dialog, SIGNAL(fileSelected(QString)), this,
            SLOT(createNonDefaultDatabaseHelper(QString)));
    }


    void MainWindow::createNonDefaultDatabaseHelper(const QString& file)
    {
        YString fileConverted(file.toStdString());

        // A demand has been made to confirm overwrite. So if file already exists,
        // delete it
        if (IO::File::Exists(fileConverted))
            IO::File::Delete(fileConverted);

        std::unique_ptr<Database::Database> ptr(new Database::Database(fileConverted,
            Database::nsTable::createAndLoad));
        pDb = std::move(ptr);

        // Close the dialog box that was asking about database creation
        pDlgNoDefaultDb->close();
        pDlgNoDefaultDb = nullptr;
    }


} // namespace Gui
} // namespace PictStock


