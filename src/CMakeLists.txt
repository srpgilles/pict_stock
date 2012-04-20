add_library(SgPhoto SHARED
	sg_photo.hpp
	photo_directory.cpp
	photo_directory.hpp
	photo_directory.hxx
	extended_photo.cpp
	extended_photo.hpp
	extended_photo.hxx
	
	private/photo_directory_iterator.cpp
	private/photo_directory_iterator.hpp
	private/tools.hpp
)


add_executable(test
	main.cc
)


target_link_libraries(test
	SgPhoto
	exiv2
	yuni-static-core
)
