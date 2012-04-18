

#
# Getting the folder where this file is located
#
set(CurrentFolder "${CMAKE_CURRENT_LIST_FILE}")
string(REPLACE "\\common-settings.cmake" "" CurrentFolder "${CurrentFolder}")
string(REPLACE "/common-settings.cmake" "" CurrentFolder "${CurrentFolder}")
# Current Folder : ${CurrentFolder}


# Common options to all GCC-based compilers
set(COMMON_GCC_OPTIONS  "-Woverloaded-virtual -Wall -Wextra -Wunused-parameter -Wconversion")
set(COMMON_GCC_OPTIONS  "${COMMON_GCC_OPTIONS} -Wmissing-noreturn -Wcast-align  -Wfloat-equal -Wundef")
set(COMMON_GCC_OPTIONS  "${COMMON_GCC_OPTIONS} -D_REENTRANT -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64")

# Common options for GCC on Unixes (mingw excluded)
# fPIC seems a good choice in most cases
set(COMMON_GCC_OPTIONS_UNIX  "${COMMON_GCC_OPTIONS} -fPIC")

include(CheckCXXCompilerFlag)
if(MINGW OR (NOT WIN32 AND NOT WIN64))
	check_cxx_compiler_flag("-std=c++0x" HAS_GCC_CPP0X_SUPPORT)
	if(HAS_GCC_CPP0X_SUPPORT)
		set(COMMON_GCC_OPTIONS  "${COMMON_GCC_OPTIONS} -std=gnu++0x")
		set(COMMON_GCC_OPTIONS_UNIX  "${COMMON_GCC_OPTIONS_UNIX} -std=gnu++0x")
	endif()
endif()


if(NOT WIN32)
	set(CMAKE_CXX_FLAGS_RELEASE         "${COMMON_GCC_OPTIONS_UNIX} -O3 -fomit-frame-pointer -fstrict-aliasing -momit-leaf-frame-pointer -fno-tree-pre -falign-loops -mfpmath=sse -msse -msse2 -Wuninitialized")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${COMMON_GCC_OPTIONS_UNIX} -O3 -fomit-frame-pointer -mfpmath=sse -msse -msse2")
	set(CMAKE_CXX_FLAGS_DEBUG           "${COMMON_GCC_OPTIONS_UNIX} -g -ggdb3")
endif()
if(MINGW)
	set(CMAKE_CXX_FLAGS_RELEASE         "${COMMON_GCC_OPTIONS} -O3 -fomit-frame-pointer -fstrict-aliasing -momit-leaf-frame-pointer -fno-tree-pre -falign-loops -mthreads -Wuninitialized")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "${COMMON_GCC_OPTIONS} -mthreads -ggdb3")
	set(CMAKE_CXX_FLAGS_DEBUG           "${COMMON_GCC_OPTIONS} -mthreads -ggdb3")
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

set(CMAKE_CXX_FLAGS_RELEASE       "${CMAKE_CXX_FLAGS_RELEASE}      ${PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS}")
set(CMAKE_CXX_FLAGS_RELWITHDEBUG  "${CMAKE_CXX_FLAGS_RELWITHDEBUG} ${PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS}")
set(CMAKE_CXX_FLAGS_DEBUG         "${CMAKE_CXX_FLAGS_DEBUG}        ${PROFILE_CXX_FLAGS_INSTRUCTIONS_SETS}")

# Override
if (NOT "${CXX_FLAGS_OVERRIDE}" STREQUAL "")
	set(CMAKE_CXX_FLAGS_RELEASE          "${CXX_FLAGS_OVERRIDE}")
	set(CMAKE_CXX_FLAGS_RELWITHDEBUG     "${CXX_FLAGS_OVERRIDE}")
	set(CMAKE_CXX_FLAGS_RELWITHDEBUGINFO "${CXX_FLAGS_OVERRIDE}")
	set(CMAKE_CXX_FLAGS_DEBUG            "${CXX_FLAGS_OVERRIDE}")
endif()

if(NOT "${CXX_FLAGS_OVERRIDE_ADD_DEBUG}" STREQUAL "")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_FLAGS_OVERRIDE_ADD_DEBUG}")
endif()
if(NOT "${CXX_FLAGS_OVERRIDE_ADD_RELEASE}" STREQUAL "")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CXX_FLAGS_OVERRIDE_ADD_RELEASE}")
endif()
if(NOT "${CXX_FLAGS_OVERRIDE_ADD_RELWITHDEBINFO}" STREQUAL "")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} ${CXX_FLAGS_OVERRIDE_ADD_RELWITHDEBINFO}")
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_RELEASE}")
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS_DEBUG}")
endif()





