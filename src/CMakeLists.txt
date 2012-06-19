cmake_minimum_required(VERSION 2.8)

add_subdirectory(extended_photo)
add_subdirectory(photo_directory)
add_subdirectory(sort_new_photos)


add_executable(PictStock
	main.cpp
)

target_link_libraries(PictStock
	sort_new_photos
)


add_executable(dev
	main_dev.cpp
)

target_link_libraries(dev
	sort_new_photos

)


add_executable(test
	main_test.cpp
)

target_link_libraries(test
	yuni-static-core
)
