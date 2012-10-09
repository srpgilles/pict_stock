
YMESSAGE_MODULE("Audio  (EXPERIMENTAL)")


LIBYUNI_CONFIG_LIB("both" "audio"      "yuni-static-audio-core")
LIBYUNI_CONFIG_DEPENDENCY("audio" "core") # yuni-core is required


#
# Windows-specific
#
if (WIN32 OR WIN64)
	LIBYUNI_CONFIG_LIB("both" "audio" ws2_32)
endif (WIN32 OR WIN64)


set(SRC_AUDIO
		audio/queueservice.h
		audio/queueservice.hxx
		audio/queueservice.cpp
		audio/loop.h
		audio/loop.cpp
		audio/emitter.h
		audio/emitter.hxx
		audio/emitter.cpp
		audio/sound.h
		audio/sound.hxx
		audio/sound.cpp
		private/audio/types.h
		private/audio/openal.h
		private/audio/openal.cpp
		private/audio/av.h
		private/audio/av.hxx
		private/audio/av.cpp
	)

include(CheckIncludeFile)

#
# OpenAL
#

# Select default OpenAL mode
if(NOT YUNI_DvP_OPENAL_MODE)
	if(WIN32 OR WIN64)
		set(YUNI_DvP_OPENAL_MODE devpack)
	else()
		set(YUNI_DvP_OPENAL_MODE system)
	endif()
endif()


YMESSAGE("      -> OpenAL Libraries: ${YUNI_DvP_OPENAL_MODE}")

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/cmake/deps/openal-${YUNI_DvP_OPENAL_MODE}.cmake)
	include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/deps/openal-${YUNI_DvP_OPENAL_MODE}.cmake)
else()
	YFATAL("[!!] Invalid YUNI_DvP_OPENAL_MODE: ${YUNI_DvP_OPENAL_MODE}")
endif()

YMESSAGE("      -> OpenAL Libraries: ${OPENAL_INCLUDE_DIR}")



#
# FFmpeg
#
YMESSAGE("Added Support for FFMpeg")
DEVPACK_IMPORT_FFMPEG()
set(SRC_AUDIO_FFMPEG ${YUNI_EXT_FFMPEG_HEADERS})

if (NOT WIN32 AND NOT WIN64)
	# ZLIB
	find_package(ZLIB)
	if (ZLIB_FOUND)
		list(APPEND YUNI_EXT_FFMPEG_LIB ${ZLIB_LIBRARIES})
	else (ZLIB_FOUND)
		YERROR(    "Impossible to find ZLib (Audio will not work properly !)")
		YMESSAGE(    " * Packages needed on debian: libz-dev")
		YMESSAGE(    " * Packages needed on redhat: zlib-devel")
	endif (ZLIB_FOUND)

	# BZIP2
	FIND_PACKAGE(BZip2)
	if(BZIP2_FOUND)
		list(APPEND YUNI_EXT_FFMPEG_LIB ${BZIP2_LIBRARIES})
	else(BZIP2_FOUND)
		YERROR(    "Impossible to find BZip2 (Audio will not work properly !)")
		YMESSAGE(    " * Packages needed on debian: libbz2-dev")
		YMESSAGE(    " * Packages needed on redhat: bzip2-devel")
	endif(BZIP2_FOUND)
endif (NOT WIN32 AND NOT WIN64)

LIBYUNI_CONFIG_LIB_RAW_COMMAND("both" "audio" "${YUNI_EXT_FFMPEG_LIB}")
LIBYUNI_CONFIG_INCLUDE_PATH("both" "audio" "${YUNI_EXT_FFMPEG_INCLUDE}")

### WARNING: FFmpeg 0.6 (and other versions) fail to compile with:
### error: 'UINT64_C' was not declared in this scope
### This define is required to solve this.
add_definitions(-D__STDC_CONSTANT_MACROS)

source_group(Audio FILES ${SRC_AUDIO})
source_group(Audio\\Ffmpeg FILES ${SRC_AUDIO_FFMPEG})

add_library(yuni-static-audio-core STATIC ${SRC_AUDIO_FFMPEG} ${SRC_AUDIO})

# Setting output path
set_target_properties(yuni-static-audio-core PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${YUNI_OUTPUT_DIRECTORY}/lib")

# Installation
install(TARGETS yuni-static-audio-core
	COMPONENT ${YUNICOMPONENT_AUDIO_CORE}
	ARCHIVE DESTINATION lib/${YUNI_VERSIONED_INST_PATH})

# Install Audio-related headers
install(
	DIRECTORY audio
	COMPONENT ${YUNICOMPONENT_AUDIO_CORE}
	DESTINATION include/${YUNI_VERSIONED_INST_PATH}/yuni
	FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hxx"
	PATTERN ".svn" EXCLUDE
	PATTERN "CMakeFiles" EXCLUDE
	PATTERN "cmake" EXCLUDE
)

target_link_libraries(yuni-static-audio-core ${OPENAL_LIBRARY})

