cmake_minimum_required(VERSION 2.8)

project(SgPhotos)

include(cmake/common-settings.cmake)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE})

include_directories(ext)
include_directories(ext/yuni/src)

add_subdirectory(ext/exiv2)
add_subdirectory(ext/yuni/src)
add_subdirectory(src)

IF( MSVC )
	install(FILES exiv2 DESTINATION ${EXECUTABLE_OUTPUT_PATH})
	install(FILES parameters.ini DESTINATION ${EXECUTABLE_OUTPUT_PATH})
ENDIF( MSVC )

if (UNIX)
	FIND_PACKAGE(Boost 1.49 COMPONENTS regex REQUIRED)
	IF (Boost_FOUND)
    	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
    	ADD_DEFINITIONS( "-DHAS_BOOST" )
	ENDIF()
endif(UNIX)

