add_library(pict_stock_database STATIC
    database.cpp
    database.hpp
    database_exceptions.hpp
    cameras.cpp
    cameras.hpp
    cameras.hxx
    photographers.cpp
    photographers.hpp
    photographers.hxx
    private/table_photographers.hpp
    photographer.cpp
    photographer.hpp
    photographer.hxx
)

target_link_libraries(pict_stock_database
    tools
    sqlite3
	yuni-static-core
)

