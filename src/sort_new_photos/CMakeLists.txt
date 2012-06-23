add_library(sort_new_photos STATIC
	
	../pict_stock.hpp
	sort_new_photos.cpp
	sort_new_photos.hpp
	private/sort_new_photos_iterator.cpp
	private/sort_new_photos_iterator.hpp
	private/sort_new_photos_iterator.hxx
	private/populate_day_folder.cpp
	private/populate_day_folder.hpp
)

target_link_libraries(sort_new_photos
	exiv2
	yuni-static-core
	photo_directory
)

if (UNIX)
	target_link_libraries(sort_new_photos ${Boost_LIBRARIES})
endif (UNIX)