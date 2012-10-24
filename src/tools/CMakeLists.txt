add_library(tools STATIC
    exceptions.cpp
    exceptions.hpp
    numeration.cpp
    numeration.hpp
    read_parameter_file.cpp
    read_parameter_file.hpp
    sqlite_wrapper.cpp
    sqlite_wrapper.hpp
    sqlite_wrapper.hxx
    exceptions/sqlite_wrapper_exceptions.hpp
    tools.hpp
    tuple_utilities.hpp
)

target_link_libraries(tools
	yuni-static-core
	sqlite3
)

