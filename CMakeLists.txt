cmake_minimum_required(VERSION 2.6)

project(SgPhotos)

include(cmake/common-settings.cmake)

set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

include_directories(ext/yuni/src)

add_subdirectory(ext/yuni/src)

add_library(SgPhoto SHARED
	src/sg_photo.cpp
	src/sg_photo.hpp

	src/photo_directory.hpp
	src/photo_directory.hxx
	
	src/private/photo_directory_iterator.hxx
	src/private/photo_directory_iterator.hpp
	src/private/tools.hpp
)


add_executable(test
	src/main.cc
)


target_link_libraries(test
	SgPhoto
	exiv2
	yuni-static-core
)




