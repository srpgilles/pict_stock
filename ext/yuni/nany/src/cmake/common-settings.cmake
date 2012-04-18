
#
# CMake Stuff
#
#set(CMAKE_VERBOSE_MAKEFILE ON)
# Policy
if(NOT CMAKE_MINOR_VERSION EQUAL 4 OR NOT CMAKE_MAJOR_VERSION EQUAL 2 )
	cmake_policy(SET CMP0004 OLD)
	cmake_policy(SET CMP0003 NEW)
endif()
include(CheckIncludeFile)

#
# Getting the folder where this file is located
#
set(CurrentFolder "${CMAKE_CURRENT_LIST_FILE}")
string(REPLACE "\\common-settings.cmake" "" CurrentFolder "${CurrentFolder}")
string(REPLACE "/common-settings.cmake" "" CurrentFolder "${CurrentFolder}")
# Current Folder : ${CurrentFolder}



# Common options to all GCC-based compilers
set(YUNI_COMMON_GCC_OPTIONS  "-Woverloaded-virtual -Wall -Wextra -Wunused-parameter -Wconversion")
set(YUNI_COMMON_GCC_OPTIONS  "${YUNI_COMMON_GCC_OPTIONS} -Wcast-align  -Wfloat-equal -Wundef")
set(YUNI_COMMON_GCC_OPTIONS  "${YUNI_COMMON_GCC_OPTIONS} -D_REENTRANT -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64")

# Common options for GCC on Unixes (mingw excluded)
# fPIC seems a good choice in most cases
set(YUNI_COMMON_GCC_OPTIONS_UNIX  "${YUNI_COMMON_GCC_OPTIONS} -fPIC")

include(CheckCXXCompilerFlag)
if(MINGW OR (NOT WIN32 AND NOT WIN64))
	check_cxx_compiler_flag("-std=c++0x" NANY_HAS_GCC_CPP0X_SUPPORT)
	if(NANY_HAS_GCC_CPP0X_SUPPORT)
		set(YUNI_COMMON_GCC_OPTIONS  "${YUNI_COMMON_GCC_OPTIONS} -std=gnu++0x")
		set(YUNI_COMMON_GCC_OPTIONS_UNIX  "${YUNI_COMMON_GCC_OPTIONS_UNIX} -std=gnu++0x")
	endif()
endif()


if(NOT WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE         "${YUNI_COMMON_GCC_OPTIONS_UNIX} -O3 -fomit-frame-pointer -fstrict-aliasing -momit-leaf-frame-pointer -fno-tree-pre -falign-loops -mfpmath=sse -msse -msse2 -Wuninitialized")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${YUNI_COMMON_GCC_OPTIONS_UNIX} -O3 -fomit-frame-pointer -mfpmath=sse -msse -msse2")
	set(CMAKE_CXX_FLAGS_DEBUG           "${YUNI_COMMON_GCC_OPTIONS_UNIX} -g -ggdb3")
endif()
if(MINGW)
	set(CMAKE_CXX_FLAGS_RELEASE         "${YUNI_COMMON_GCC_OPTIONS} -O3 -fomit-frame-pointer -fstrict-aliasing -momit-leaf-frame-pointer -fno-tree-pre -falign-loops -mthreads -Wuninitialized")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${YUNI_COMMON_GCC_OPTIONS} -mthreads -g2")
	set(CMAKE_CXX_FLAGS_DEBUG           "${YUNI_COMMON_GCC_OPTIONS} -mthreads -g2")
endif()
if(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE         "/EHsc /Ox /Ob2 /Ot /O2 /Oy /MD /GS- /Gy /arch:SSE2")
	set(CMAKE_CXX_FLAGS_DEBUG           "/EHsc /Od /GR /GS /RTC1 /RTCc /Ob1 /Ot /MDd /W3")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "/EHsc /Ox /Ob2 /Ot /O2 /Oy /MDd /GS- /Gy")
endif()
if(APPLE)
	# The SSE instruction set is mandatory
	set(CMAKE_CXX_FLAGS_RELEASE         "${CMAKE_CXX_FLAGS_RELEASE} -msse2 -fvisibility=hidden")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${CMAKE_CXX_FLAGS_RELEASE} -msse2 -fvisibility=hidden")
	set(CMAKE_CXX_FLAGS_DEBUG           "${CMAKE_CXX_FLAGS_DEBUG}   -msse2 -gfull -fvisibility=hidden")
endif()

set(CMAKE_CXX_FLAGS_RELEASE       "${CMAKE_CXX_FLAGS_RELEASE}      ${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBUG  "${CMAKE_CXX_FLAGS_RELWITHDEBUG} ${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS}")
set(CMAKE_CXX_FLAGS_DEBUG         "${CMAKE_CXX_FLAGS_DEBUG}        ${YUNI_PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS}")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
endif()


#
# Extra - Mac OS X Bundles
#
if(APPLE)
	set(MACOSX_BUNDLE_COPYRIGHT "Nany compiler")
	if(YUNI_MACOX_UNIVERSAL_BINARIES)
		NMESSAGE("Enabled universal binaries (custom : ${YUNI_MACOX_UNIVERSAL_BINARIES})")
		set(CMAKE_OSX_ARCHITECTURES "${YUNI_MACOX_UNIVERSAL_BINARIES}") # ppc;i386;ppc64;x86_64
	else()
		NMESSAGE("Enabled universal binaries (i386, x86_64)")
		set(CMAKE_OSX_ARCHITECTURES "i686;x86_64") # ppc;i386;ppc64;x86_64
	endif()
endif()


