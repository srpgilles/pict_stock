add_library(pict_frame STATIC
	pict_frame.cpp
	pict_frame.hpp
	private/scan_photo_directory.cpp
	private/scan_photo_directory.hpp
)

target_link_libraries(pict_frame
	photo_directory
	exiv2
	yuni-static-core
)

if (UNIX)
	target_link_libraries(pict_frame ${Boost_LIBRARIES})
endif (UNIX)