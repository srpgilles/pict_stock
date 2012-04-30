
YMESSAGE("")
YMESSAGE("Modules")
YMESSAGE("")


# The target directory for static libs
Set(LIBRARY_OUTPUT_PATH      "${CMAKE_CURRENT_BINARY_DIR}/../build/${YUNI_TARGET}/lib")
Set(ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/../build/${YUNI_TARGET}/lib")



#
# Preflight
#
Set(YUNI_COMPILED_WITH_SUPPORT_FOR_OPENGL  0)
Set(YUNI_COMPILED_WITH_SUPPORT_FOR_DIRECTX 0)
Set(YUNI_CMAKE_ERROR 0)


#
# Informations for yuni-config
#
Set(LIBYUNI_CONFIG_ENABLED true)


include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/YuniConfig.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/common-settings.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/../cmake/modules.cmake")




if(YUNI_MODULE_CORE)
	# The header yuni.h must be available
	CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.cmake" "${CMAKE_CURRENT_BINARY_DIR}/config.h")
	#
	if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/yuni.h" AND EXISTS "${CMAKE_CURRENT_BINARY_DIR}/core/string.h")
		# The file config.h is at the same place that the sources.
		# Consequently there is no need to specify a path
		# This is necessary to help distcc as much as possible  (see #340)
	else()
		add_definitions("-DYUNI_CONFIG_H_LOCATION=\"${CMAKE_CURRENT_BINARY_DIR}/config.h\"")
	endif()
	# Core
	include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/core.cmake")
endif()

if(YUNI_MODULE_VM)
	include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/vm.cmake")
endif()


if(YUNI_MODULE_VFS)
	include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/vfs.cmake")
endif(YUNI_MODULE_VFS)


if(YUNI_MODULE_DEVICES)
	LIBYUNI_CONFIG_LIB("both" "display"    "yuni-static-device-display")
	LIBYUNI_CONFIG_LIB("both" "mouse"      "yuni-static-device-mouse")
	LIBYUNI_CONFIG_DEPENDENCY("mouse" "devices")
	LIBYUNI_CONFIG_LIB("both" "keyboard"   "yuni-static-device-keyboard")
	LIBYUNI_CONFIG_DEPENDENCY("keyboard" "devices")

	if(YUNI_MODULE_DEVICE_DISPLAY)
		include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/device-display.cmake")
	endif()
	if(YUNI_MODULE_DEVICE_KEYBOARD)
	endif()
	if(YUNI_MODULE_DEVICE_MOUSE)
	endif()
endif()



#
# Module : Network
#
If(YUNI_MODULE_NET)
	include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/net.cmake")

	if(YUNI_MODULE_NET_MESSAGES)
		include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/net-messages.cmake")
	endif()
endif()

if(YUNI_MODULE_LDO)
	include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/ldo.cmake")
endif()

if(YUNI_MODULE_GRAPHICS)
	if (YUNI_MODULE_OPENGL)
		include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/graphics-opengl.cmake")
	endif()
endif()

if(YUNI_MODULE_UI)
	include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/ui.cmake")
endif()



if(YUNI_MODULE_AUDIO)
	include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/audio.cmake")
endif()


if(YUNI_MODULE_ALGORITHMS)
	include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/algorithms.cmake")
endif(YUNI_MODULE_ALGORITHMS)


if(YUNI_MODULE_DOCUMENTATION)
	include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/documentation.cmake")
endif()


if(YUNI_MODULE_EXTRA_UUID)
	include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/extra-uuid.cmake")
endif()



if(YUNI_TESTS)
	include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/test.cmake")
endif()


# Generating config.h
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config.h)
# Generating version
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/version.cmake ${CMAKE_CURRENT_SOURCE_DIR}/yuni.version)


#
# Misc
#
add_definitions("-DYUNI_LIBRARY_STATIC")
LIBYUNI_CONFIG_DEFINITION("both" "core" "YUNI_LIBRARY_STATIC")
include_directories(${YUNI_INCLUDE})



YMESSAGE("")

if("${YUNI_CMAKE_ERROR}" GREATER 0)
	YMESSAGE("")
	YERROR("An error has occured. Aborting.")
	YMESSAGE("")
	message(FATAL_ERROR  "Aborting.")
endif()

