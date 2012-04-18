
#
# --- Modules - Default settings ---
#
if (WIN32 OR WIN64)
	# NSIS does not support semicolons.....
	set(YUNICOMPONENT_CORE              "yuni_core")
	set(YUNICOMPONENT_ALGORITHMS        "yuni_algorithms")
	set(YUNICOMPONENT_AUDIO_CORE        "yuni_audio_core")
	set(YUNICOMPONENT_DEVICE_DISPLAY    "yuni_device_display")
	set(YUNICOMPONENT_UUID              "yuni_uuid")
else()
	set(YUNICOMPONENT_CORE              "yuni-core")
	set(YUNICOMPONENT_ALGORITHMS        "yuni-algorithms")
	set(YUNICOMPONENT_AUDIO_CORE        "yuni-audio-core")
	set(YUNICOMPONENT_DEVICE_DISPLAY    "yuni-device-display")
	set(YUNICOMPONENT_DEVICE_DISPLAY    "yuni-uuid")
endif()




# Core
set(YUNI_MODULE_CORE                      true) # Must be True

# VFS
#set(YUNI_MODULE_VFS                       false)
#	set(YUNI_MODULE_VFS_FILE              true)

# VM
set(YUNI_MODULE_VM                        false)

# Devices
set(YUNI_MODULE_DEVICES                   false)
	set(YUNI_MODULE_DEVICE_DISPLAY        true)
	set(YUNI_MODULE_DEVICE_KEYBOARD       true)
	set(YUNI_MODULE_DEVICE_MOUSE          true)

# Audio
set(YUNI_MODULE_AUDIO                     false)

# Network
set(YUNI_MODULE_NET                       false)
set(YUNI_MODULE_NET_MESSAGES              false)

# LDO
set(YUNI_MODULE_LDO                       false)

# Graphics
set(YUNI_MODULE_GRAPHICS                  false)
set(YUNI_MODULE_OPENGL                    false)

# UI (User Interface)
set(YUNI_MODULE_UI                        false)

# Algorithms
set(YUNI_MODULE_ALGORITHMS                false)

# UUID
set(YUNI_MODULE_EXTRA_UUID                false)

# Doc
set(YUNI_MODULE_DOCUMENTATION             false)



# Tests
set(YUNI_TESTS   false)



# The list of all available modules
# There is no need for `core`, which is implicit
set(YUNI_MODULE_LIST
	algorithms
	vm
	vfs
		vfs-local
	audio
	devices
		display
		keyboard
		mouse
	graphics
		opengl
	ui
	net
		netserver
		netclient
	ldo
	# extra
		uuid
	docs
	)




#
# --- Command lines options ---
#
if(MODULES)
	set(KeywordError false)
	string(REPLACE "," ";" MODULES "${MODULES}")
	string(REPLACE " " ";" MODULES "${MODULES}")
	string(REPLACE "+" "" MODULES "${MODULES}")

	foreach(it ${MODULES})
		set(KeywordIsKnown false)

		# core
		if("${it}" STREQUAL "core")
			set(YUNI_MODULE_CORE true)
			set(KeywordIsKnown true)
		endif()
		# -core
		if("${it}" STREQUAL "-core")
			set(KeywordIsKnown true)
			YMESSAGE("[!!] Module: Impossible to disable the core module")
			set(KeywordError true)
		endif()

		# all
		if("${it}" STREQUAL "all")
			set(YUNI_MODULE_CORE true)
			#set(YUNI_MODULE_VFS true)
			set(YUNI_MODULE_DEVICES true)
			set(YUNI_MODULE_VM true)
			set(YUNI_MODULE_AUDIO true)
			set(YUNI_MODULE_NET true)
			set(YUNI_MODULE_NET_MESSAGES true)
			set(YUNI_MODULE_GRAPHICS true)
			set(YUNI_MODULE_OPENGL true)
			set(YUNI_MODULE_UI true)
			set(YUNI_MODULE_ALGORITHMS true)
			set(YUNI_MODULE_EXTRA_UUID true)
			set(YUNI_MODULE_LDO true)
			set(YUNI_MODULE_DOCUMENTATION true)
			set(YUNI_TESTS true)
			set(KeywordIsKnown true)
		endif()

		# vfs
		#if("${it}" STREQUAL "vfs")
		#	set(YUNI_MODULE_VFS true)
		#	set(KeywordIsKnown true)
		#endif()
		# -vfs
		#if("${it}" STREQUAL "-vfs")
		#	set(YUNI_MODULE_VFS false)
		#	set(KeywordIsKnown true)
		#endif()

		# vfs-local
		#if("${it}" STREQUAL "vfs-local")
		#	set(YUNI_MODULE_VFS true)
		#	set(YUNI_MODULE_VFS_LOCAL true)
		#	set(KeywordIsKnown true)
		#endif()
		# -vfs
		#if("${it}" STREQUAL "-vfs-local")
		#	set(YUNI_MODULE_VFS_LOCAL false)
		#	set(KeywordIsKnown true)
		#endif()

		# vm
		if("${it}" STREQUAL "vm")
			set(YUNI_MODULE_VM true)
			set(KeywordIsKnown true)
		endif()
		# -vm
		if("${it}" STREQUAL "-vm")
			set(YUNI_MODULE_VM false)
			set(KeywordIsKnown true)
		endif()

		# ldo
		if("${it}" STREQUAL "ldo")
			set(YUNI_MODULE_LDO true)
			set(KeywordIsKnown true)
		endif()
		# -ldo
		if("${it}" STREQUAL "-ldo")
			set(YUNI_MODULE_LDO false)
			set(KeywordIsKnown true)
		endif()

		# algorithms
		if("${it}" STREQUAL "algorithms")
			set(YUNI_MODULE_ALGORITHMS true)
			set(KeywordIsKnown true)
		endif()
		# -algorithms
		if("${it}" STREQUAL "-algorithms")
			set(YUNI_MODULE_ALGORITHMS false)
			set(KeywordIsKnown true)
		endif()


		# display
		if("${it}" STREQUAL "display")
			set(YUNI_MODULE_DEVICE_DISPLAY true)
			set(KeywordIsKnown true)
		endif()
		# -display
		if("${it}" STREQUAL "-display")
			set(YUNI_MODULE_DEVICE_DISPLAY false)
			set(KeywordIsKnown true)
		endif()

		# keyboard
		if("${it}" STREQUAL "keyboard")
			set(YUNI_MODULE_DEVICE_KEYBOARD true)
			set(KeywordIsKnown true)
		endif()
		# -keyboard
		if("${it}" STREQUAL "-keyboard")
			set(YUNI_MODULE_DEVICE_KEYBOARD false)
			set(KeywordIsKnown true)
		endif()

		# mouse
		if("${it}" STREQUAL "mouse")
			set(YUNI_MODULE_DEVICE_MOUSE true)
			set(KeywordIsKnown true)
		endif()
		# -mouse
		if("${it}" STREQUAL "-mouse")
			set(YUNI_MODULE_DEVICE_MOUSE false)
			set(KeywordIsKnown true)
		endif()

		# devices
		if("${it}" STREQUAL "devices")
			set(YUNI_MODULE_DEVICES true)
			set(KeywordIsKnown true)
		endif()
		# -devices
		if("${it}" STREQUAL "-devices")
			set(YUNI_MODULE_DEVICES false)
			set(KeywordIsKnown true)
		endif()


		# net
		if("${it}" STREQUAL "net")
			set(YUNI_MODULE_NET true)
			set(KeywordIsKnown true)
		endif()
		# -net
		if("${it}" STREQUAL "-net")
			set(YUNI_MODULE_NET false)
			set(KeywordIsKnown true)
		endif()

		# net-messages
		if("${it}" STREQUAL "net-messages")
			set(YUNI_MODULE_NET_MESSAGES true)
			set(KeywordIsKnown true)
		endif()
		# -net-messages
		if("${it}" STREQUAL "-net-messages")
			set(YUNI_MODULE_NET_MESSAGES false)
			set(KeywordIsKnown true)
		endif()

		# audio
		if("${it}" STREQUAL "audio")
			set(KeywordIsKnown true)
			set(YUNI_MODULE_AUDIO true)
		endif()
		# -audio
		if("${it}" STREQUAL "-audio")
			set(KeywordIsKnown true)
			set(YUNI_MODULE_AUDIO false)
		endif()


		# Tests
		if("${it}" STREQUAL "tests")
			set(KeywordIsKnown true)
			set(YUNI_TESTS true)
		endif()
		# -tests
		if("${it}" STREQUAL "-tests")
			set(KeywordIsKnown true)
			set(YUNI_TESTS false)
		endif()

		# graphics (Graphic renderers)
		if("${it}" STREQUAL "graphics")
			set(KeywordIsKnown true)
			set(YUNI_MODULE_GRAPHICS true)
		endif()
		# -graphics
		if("${it}" STREQUAL "-graphics")
			set(KeywordIsKnown true)
			set(YUNI_MODULE_GRAPHICS false)
		endif()

		# OpenGL renderer
		if("${it}" STREQUAL "opengl")
			set(KeywordIsKnown true)
			set(YUNI_MODULE_OPENGL true)
		endif()
		# -opengl
		if("${it}" STREQUAL "-opengl")
			set(KeywordIsKnown true)
			set(YUNI_MODULE_OPENGL false)
		endif()

		# ui (User Interface)
		if("${it}" STREQUAL "ui")
			set(KeywordIsKnown true)
			set(YUNI_MODULE_UI true)
		endif()
		# -ui
		if("${it}" STREQUAL "-ui")
			set(KeywordIsKnown true)
			set(YUNI_MODULE_UI false)
		endif()

		# uuid
		if("${it}" STREQUAL "uuid")
			set(KeywordIsKnown true)
			set(YUNI_MODULE_EXTRA_UUID true)
		endif()
		# -uuid
		if("${it}" STREQUAL "-uuid")
			set(KeywordIsKnown true)
			set(YUNI_MODULE_EXTRA_UUID false)
		endif()

		# docs
		if("${it}" STREQUAL "docs" OR "${it}" STREQUAL "doc")
			set(KeywordIsKnown true)
			set(YUNI_MODULE_DOCUMENTATION true)
		endif()
		# -docs
		if("${it}" STREQUAL "-doc" OR "${it}" STREQUAL "-docs")
			set(KeywordIsKnown true)
			set(YUNI_MODULE_DOCUMENTATION false)
		endif()

		if(NOT KeywordIsKnown)
			YMESSAGE("[!!] Unknown module from command line: `${it}` (ignored)")
			set(KeywordError true)
		endif()

	endforeach()

	if(KeywordError)
		YMESSAGE("")
		YMESSAGE("Errors on modules. Here is the list of all available modules :")
		YMESSAGE("(+ : Enable the module,  - disable the module)")
		YMESSAGE(" Main and virtual modules")
		YMESSAGE("    +core          : The core module (needed)")
		YMESSAGE("    -/+tests       : Atomic Tests for the yuni framework")
		YMESSAGE("    +all           : Enable all main modules (ui,tests,...)")
		YMESSAGE("    -/+docs        : Documentation Tools")
		#YMESSAGE(" The VFS module")
		#YMESSAGE("    -/+vfs         : The Virtual filesystem")
		#YMESSAGE("    -/+vfs-local   : Support for the local filesystems")
		YMESSAGE(" The device modules")
		YMESSAGE("    -/+devices     : All devices (display,keyboard,mouse...)")
		YMESSAGE("    -/+display     : The Display device")
		YMESSAGE("    -/+keyboard    : The Keyboard device")
		YMESSAGE("    -/+mouse       : The Mouse device")
		YMESSAGE(" The audio modules")
		YMESSAGE("    -/+audio       : The Audio module (default: disabled)")
		YMESSAGE(" The graphics modules")
		YMESSAGE("    -/+opengl      : The OpenGL renderer module (default: disabled)")
		YMESSAGE(" The ui modules")
		YMESSAGE("    -/+ui          : The ui module (default: disabled)")
		YMESSAGE(" The virtual machine module")
		YMESSAGE("    -/+vm          : The Virtual machine")
		YMESSAGE(" The extra modules")
		YMESSAGE("    -/+uuid        : UUID (default: disabled)")
		YMESSAGE(" The algorithms")
		YMESSAGE("    -/+algorithms  : Standard algorithms")
		YMESSAGE("")
		message(FATAL_ERROR "Errors on module names")
	endif()
endif()


#
# Dependencies
#
if(YUNI_MODULE_OPENGL)
	set(YUNI_MODULE_GRAPHICS true)
endif()
if(YUNI_MODULE_UI)
	set(YUNI_MODULE_DEVICES true)
	set(YUNI_MODULE_DEVICE_DISPLAY true)
endif()
if(YUNI_MODULE_LDO)
	set(YUNI_MODULE_NET_MESSAGES true)
endif()
if(YUNI_MODULE_NET_MESSAGES)
	set(YUNI_MODULE_NET true)
endif()







#
# List of all available modules
#

set(YUNI_MODULE_AVAILABLE)

if(YUNI_MODULE_ALGORITHMS)
	list(APPEND YUNI_MODULE_AVAILABLE algorithms)
endif()

if(YUNI_MODULE_DEVICES)
	list(APPEND YUNI_MODULE_AVAILABLE devices)
	if(YUNI_MODULE_DEVICE_DISPLAY)
		list(APPEND YUNI_MODULE_AVAILABLE display)
	endif()
	if(YUNI_MODULE_DEVICE_MOUSE)
		list(APPEND YUNI_MODULE_AVAILABLE mouse)
	endif()
	if(YUNI_MODULE_DEVICE_KEYBOARD)
		list(APPEND YUNI_MODULE_AVAILABLE keyboard)
	endif()
endif()

if(YUNI_MODULE_VFS)
	list(APPEND YUNI_MODULE_AVAILABLE vfs)
	if(YUNI_MODULE_VFS_FILE)
		list(APPEND YUNI_MODULE_AVAILABLE vfs-local)
	endif()
endif()

if(YUNI_MODULE_VM)
	list(APPEND YUNI_MODULE_AVAILABLE vm)
endif()



if(YUNI_MODULE_NET)
	list(APPEND YUNI_MODULE_AVAILABLE net)
	if(YUNI_MODULE_NET_MESSAGES)
		list(APPEND YUNI_MODULE_AVAILABLE net-messages)
	endif()
endif()

if(YUNI_MODULE_LDO)
	list(APPEND YUNI_MODULE_AVAILABLE ldo)
endif()


if(YUNI_MODULE_AUDIO)
	list(APPEND YUNI_MODULE_AVAILABLE audio)
endif()

if(YUNI_MODULE_GRAPHICS)
	list(APPEND YUNI_MODULE_AVAILABLE graphics)
	if (YUNI_MODULE_OPENGL)
		list(APPEND YUNI_MODULE_AVAILABLE opengl)
	endif()
endif()

if(YUNI_MODULE_UI)
	list(APPEND YUNI_MODULE_AVAILABLE ui)
endif()

if(YUNI_MODULE_DOCUMENTATION)
	LIST(APPEND YUNI_MODULE_AVAILABLE docs)
endif()

if(YUNI_MODULE_EXTRA_UUID)
	LIST(APPEND YUNI_MODULE_AVAILABLE uuid)
endif()
