add_library(tools STATIC
	exceptions.cpp
	exceptions.hpp
	tools.hpp
	read_parameter_file.hpp
	read_parameter_file.cpp
	numeration.cpp
	numeration.hpp
	sqlite_wrapper.cpp
	sqlite_wrapper.hpp
	sqlite_wrapper.hxx
)

target_link_libraries(tools
	yuni-static-core
	sqlite3
)

