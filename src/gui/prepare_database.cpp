#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>

#include "prepare_database.hpp"

#include <yuni/io/directory/system.h>

#include "../database/exceptions/database.hpp"

using namespace Yuni;

namespace PictStock
{
namespace Gui
{
namespace Private
{
    namespace
    {
        namespace LoadDatabaseStatus
        {
            enum Values
            {
                ok,
                does_not_exist,
                exist_but_invalid,
                no_default_location // mostly if it wasn't possible to find home directory
            };
        }


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


        /*!
         * \brief Determine the status after trying to load default database
         *
         * \param[out] out If the status is ok, pointer to the corresponding database.
         * If not, nullptr
         */

        LoadDatabaseStatus::Values determineStatus(Database::Database*& out)
        {
            out = nullptr;

            YString defaultDatabasePath;

            if (defaultDb3Location(defaultDatabasePath))
            {
                if (IO::File::Exists(defaultDatabasePath))
                {
                    try
                    {
                        Database::Database* ptr(
                            new Database::Database(defaultDatabasePath, Database::nsTable::load));
                        out = std::move(ptr);
                        return LoadDatabaseStatus::ok;
                    }
                    catch(const Database::Exceptions::DatabaseException& e)
                    {
                        return LoadDatabaseStatus::exist_but_invalid;
                    }
                }
                else
                    return LoadDatabaseStatus::does_not_exist;
            }
            else
                return LoadDatabaseStatus::no_default_location;
        }


    } // namespace anonymous




    PrepareDatabase::PrepareDatabase()
        : ::QWidget(nullptr),
          pDialog(nullptr)
    { }



    void PrepareDatabase::Init()
    {
        // First check the default emplacement; if it exists load it
        Database::Database* db;
        LoadDatabaseStatus::Values status = determineStatus(db);

        if (status == LoadDatabaseStatus::ok)
        {
            emit databaseInitialised(db);
            return;
        }

        // If it doesn't exist, prepare the dialog box
        pDialog = new QDialog(this);

        // TODO Following line doesn't work at the moment; see that later
        pDialog->setContextMenuPolicy(Qt::PreventContextMenu);
        pDialog->setModal(true);

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
        case LoadDatabaseStatus::no_default_location:
            question = new QLabel(tr("Unable to determine default location; what do you want to do?"));
            break;
        case LoadDatabaseStatus::ok:
            assert("Check the code; it shouldn't happen" && false);
        }


        dialogLayout->addWidget(question);
        optionsLayout->addWidget(createNonDefault);
        optionsLayout->addWidget(loadAnother);
        dialogLayout->addLayout(optionsLayout);

        pDialog->setLayout(dialogLayout);
        pDialog->show();

        connect(createDefault, SIGNAL(clicked()), this, SLOT(createDefaultDatabase()));
        //connect(createDefault, SIGNAL(clicked()), pDialog, SLOT(accept()));

        connect(createNonDefault, SIGNAL(clicked()), this, SLOT(createNonDefaultDatabase()));

        connect(loadAnother, SIGNAL(clicked()), this, SLOT(loadDatabase()));
    }


    void PrepareDatabase::createDefaultDatabase()
    {
        YString defaultPath;
        if (!defaultDb3Location(defaultPath))
        {
            QMessageBox::critical(this, QString(), tr("Unable to determine default location; please choose another option."));
            return;
        }

        Database::Database* ptr = new Database::Database(defaultPath,
            Database::nsTable::createAndLoad);

        // Emit signal that gives informations the db is correctly set
        emit databaseInitialised(ptr);
    }


    void PrepareDatabase::createNonDefaultDatabase()
    {
        QFileDialog* pFileDialog = new QFileDialog(pDialog, QString(), QString(),
            tr("Sqlite database file (*.db3)"));
        pFileDialog->setModal(true);
        pFileDialog->setDefaultSuffix("db3");
        pFileDialog->setAcceptMode(QFileDialog::AcceptSave);
        pFileDialog->show();

        connect(pFileDialog, SIGNAL(fileSelected(QString)), this,
            SLOT(createNonDefaultDatabaseHelper(QString)));
    }


    void PrepareDatabase::createNonDefaultDatabaseHelper(const QString& file)
    {
        YString fileConverted(file.toStdString());

        // A demand has been made to confirm overwrite. So if file already exists,
        // delete it
        if (IO::File::Exists(fileConverted))
            IO::File::Delete(fileConverted);

        Database::Database* ptr = new Database::Database(fileConverted,
            Database::nsTable::createAndLoad);

        // Close the dialog box that was asking about database creation
        //pDialog->close();

        // Emit signal that gives informations the db is correctly set
        emit databaseInitialised(ptr);
    }


    void PrepareDatabase::loadDatabase()
    {
        auto file = QFileDialog::getOpenFileName(pDialog, QString(),
            QString(), "Sqlite database (*.db3)");

        if (file.isEmpty())
            return;

        YString fileConverted(file.toStdString());
        Database::Database* ptr = new Database::Database(fileConverted,
            Database::nsTable::load);

        // Close the dialog box that was asking about database creation
        pDialog->close();

        // Emit signal that gives informations the db is correctly set
        emit databaseInitialised(ptr);  
    }



} // namespace Private
} // namespace Gui
} // namespace PictStock

