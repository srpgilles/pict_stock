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
    exceptions/sqlite_wrapper_exceptions.cpp
    exceptions/sqlite_wrapper_exceptions.hpp
    tools.hpp
    tuple_utilities.hpp
    stl.hpp
)

target_link_libraries(tools
	yuni-static-core
	sqlite3
)


add_executable(automatic_include_guards
    main_automatic_include_guards.cpp
)

target_link_libraries(automatic_include_guards
    yuni-static-core
)
