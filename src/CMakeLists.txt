add_library(pict_stock STATIC
	
	pict_stock.hpp
	photo_directory.cpp
	photo_directory.hpp
	photo_directory.hxx
	
	extended_photo/extended_photo.cpp
	extended_photo/extended_photo.hpp
	extended_photo/extended_photo.hxx
	extended_photo/date.cpp
	extended_photo/date.hpp
	extended_photo/relevant_informations.cpp
	extended_photo/relevant_informations.hpp
	
	private/date_tools.hxx
	sort_new_photos.cpp
	sort_new_photos.hpp
	private/traits/traits.cpp
	private/traits/traits.hpp
	extended_photo/photographer.cpp
	extended_photo/photographer.hpp
	extended_photo/photographer.hxx
	private/photo_directory_iterator.cpp
	private/photo_directory_iterator.hpp
	private/sort_new_photos_iterator.cpp
	private/sort_new_photos_iterator.hpp
	private/sort_new_photos_iterator.hxx
	private/populate_day_folder.cpp
	private/populate_day_folder.hpp
	private/tools.hpp
	private/path_format.cpp
	private/path_format.hpp
	
	
)


add_executable(PictStock
	main.cpp
)

target_link_libraries(PictStock
	pict_stock
	exiv2
	boost_regex
	yuni-static-core
)


add_executable(dev
	main_dev.cpp
)

target_link_libraries(dev
	pict_stock
	exiv2
	boost_regex
	yuni-static-core
)


add_executable(test
	main_test.cpp
)

target_link_libraries(test
	yuni-static-core
)
