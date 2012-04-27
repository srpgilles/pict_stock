cmake_minimum_required(VERSION 2.6)

project(SgPhotos)

include(cmake/common-settings.cmake)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})

include_directories(ext/yuni/src)
include_directories(ext)

add_subdirectory(ext/exiv2)
add_subdirectory(ext/yuni/src)
add_subdirectory(src)
