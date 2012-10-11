cmake_minimum_required(VERSION 2.8)

add_library(sqlite3 STATIC
	../yuni/src/tools/yuni-docmake/sqlite/sqlite3.c
	../yuni/src/tools/yuni-docmake/sqlite/sqlite3.h
	../yuni/src/tools/yuni-docmake/sqlite/sqlite3ext.h
)

if(UNIX AND NOT APPLE)
	# The DL library is required on Unixes for using SQlite
	target_link_libraries(sqlite3  dl)
endif()