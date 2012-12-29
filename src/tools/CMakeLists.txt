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
    associative_vector.hpp
    associative_vector.hxx
    tools.hpp
    tuple_utilities.hpp
    tuple_utilities_msvc/tuple_utilities.hpp
    tuple_utilities_msvc/tuple_utilities_n1.hpp
    tuple_utilities_msvc/tuple_utilities_n2.hpp
    tuple_utilities_msvc/tuple_utilities_n3.hpp
    tuple_utilities_msvc/tuple_utilities_n4.hpp
    tuple_utilities_msvc/tuple_utilities_n5.hpp
    tuple_utilities_msvc/tuple_utilities_n6.hpp
    tuple_utilities_msvc/tuple_utilities_n7.hpp
    tuple_utilities_msvc/tuple_utilities_n8.hpp
    tuple_utilities_msvc/tuple_utilities_n9.hpp
    tuple_utilities_msvc/tuple_utilities_n10.hpp
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


if (NOT MSVC)
add_executable(benchmark_associative_vector
	main_benchmark_associative_vector.cpp
)

target_link_libraries(benchmark_associative_vector
	yuni-static-core
)
endif()
