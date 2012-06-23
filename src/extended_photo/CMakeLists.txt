add_library(extended_photo STATIC
	
	../pict_stock.hpp
	extended_photo.cpp
	extended_photo.hpp
	extended_photo.hxx
	date.cpp
	date.hpp
	date.hxx
	path_informations.cpp
	path_informations.hpp
	path_informations.hxx
	photographer.cpp
	photographer.hpp
	photographer.hxx
	private/date_helpers.hpp
	private/photographer_helpers.hpp
)

target_link_libraries(extended_photo
	exiv2
	yuni-static-core
)

if (UNIX)
	target_link_libraries(extended_photo ${Boost_LIBRARIES})
endif (UNIX)