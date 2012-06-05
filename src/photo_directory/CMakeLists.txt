add_library(photo_directory STATIC
	
	../pict_stock.hpp
	photo_directory.cpp
	photo_directory.hpp
	photo_directory.hxx
	path_format.cpp
	path_format.hpp
	private/photo_directory_iterator.cpp
	private/photo_directory_iterator.hpp
	private/traits/traits.cpp
	private/traits/traits.hpp
	
)

target_link_libraries(photo_directory
	exiv2
	yuni-static-core
)

if (UNIX)
	target_link_libraries(photo_directory boost_regex)
endif (UNIX)