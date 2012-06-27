add_library(tools STATIC
	exceptions.cpp
	exceptions.hpp
	tools.hpp
	read_parameter_file.hpp
	read_parameter_file.cpp
)

target_link_libraries(tools
	yuni-static-core
)

