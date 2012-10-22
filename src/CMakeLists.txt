cmake_minimum_required(VERSION 2.8)

add_subdirectory(extended_photo)
add_subdirectory(photo_directory)
add_subdirectory(sort_new_photos)
add_subdirectory(pict_frame)
add_subdirectory(tools)



add_executable(PictStock
	main.cpp
)

target_link_libraries(PictStock
	sort_new_photos
	tools
)


add_executable(PictFrame
	main_pict_frame.cpp
)

target_link_libraries(PictFrame
	tools
	photo_directory
	pict_frame
	${QT_LIBRARIES}
)


add_executable(createPhotographerDatabase
	main_create_photographer_database.cpp
)

target_link_libraries(createPhotographerDatabase
	tools
	sqlite3
)


add_executable(dev
	main_dev.cpp
)

target_link_libraries(dev
	extended_photo
	tools
	sqlite3
	yuni-static-core
)


add_executable(test
	main_test.cpp
)

target_link_libraries(test
	yuni-static-core
)
