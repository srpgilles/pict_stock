# include <cassert>

#include "main_window.hpp"
#include "../tools/sqlite_wrapper.hpp"
#include <yuni/core/string.h>
#include <yuni/io/directory/system.h>
#include <yuni/io/file/file.h>

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
                // Load the file found
                if (loadSqliteFile(defaultDatabasePath))
                    return;
                // TODO Foresee procedure if sqlite file does not match expected format
            }
        }

        // TODO Default doesn't work; open dialog to either load another file or create a raw
        // database

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


