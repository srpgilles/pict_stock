add_library(extended_photo STATIC
	
	../pict_stock.hpp
	extended_photo.cpp
	extended_photo.hpp
	extended_photo.hxx
	date.cpp
	date.hpp
	path_informations.cpp
	path_informations.hpp
	photographer.cpp
	photographer.hpp
	photographer.hxx
)

target_link_libraries(extended_photo
	exiv2
	boost_regex
	yuni-static-core
)
