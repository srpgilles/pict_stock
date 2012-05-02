add_library(pict_stock STATIC
	pict_stock.hpp
	photo_directory.cpp
	photo_directory.hpp
	photo_directory.hxx
	extended_photo.cpp
	extended_photo.hpp
	extended_photo.hxx
	sort_new_photos.cpp
	sort_new_photos.hpp
	photographer.cpp
	photographer.hpp
	photographer.hxx
	private/photo_directory_iterator.cpp
	private/photo_directory_iterator.hpp
	private/sort_new_photos_iterator.cpp
	private/sort_new_photos_iterator.hpp
	private/sort_new_photos_iterator.hxx
	private/populate_day_folder.cpp
	private/populate_day_folder.hpp
	private/date_tools.hxx
	private/tools.hpp
)


add_executable(test
	main.cc
)


target_link_libraries(test
	pict_stock
	exiv2
	yuni-static-core
)
