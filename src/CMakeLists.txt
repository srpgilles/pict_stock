add_subdirectory(extended_photo)
add_subdirectory(photo_directory)



add_library(pict_stock STATIC
	
	pict_stock.hpp

	
	sort_new_photos.cpp
	sort_new_photos.hpp
	private/traits/traits.cpp
	private/traits/traits.hpp

	private/sort_new_photos_iterator.cpp
	private/sort_new_photos_iterator.hpp
	private/sort_new_photos_iterator.hxx
	private/populate_day_folder.cpp
	private/populate_day_folder.hpp
	private/tools.hpp
	private/path_format.cpp
	private/path_format.hpp
)


target_link_libraries(pict_stock
	extended_photo
	photo_directory
)

if (UNIX)
	target_link_libraries(pict_stock boost_regex)
endif (UNIX)


add_executable(PictStock
	main.cpp
)

target_link_libraries(PictStock
	pict_stock
)


add_executable(dev
	main_dev.cpp
)

target_link_libraries(dev
	pict_stock

)


add_executable(test
	main_test.cpp
)

target_link_libraries(test
	yuni-static-core
)
