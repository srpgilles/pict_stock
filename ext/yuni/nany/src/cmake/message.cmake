
set(NANY_ERROR_HAS_OCCURED   false)


macro(NMESSAGE msg)
	if(UNIX)
		message(STATUS "[1;30m{nany}[0m  ${msg}")
	else()
		message(STATUS "{nany}  ${msg}")
	endif()

endmacro()


macro(NMESSAGE_BOLD msg)
	if(UNIX)
		message(STATUS "[1;30m{nany}[0m  [1;37m[1m${msg}[0m")
	else()
		message(STATUS "{nany}  ${msg}")
	endif()
endmacro()


macro(NMESSAGE_TITLE msg1 msg2)
	if(UNIX)
		message(STATUS "[1;30m{nany}[0m  [1;32m${msg1}[0m[1;37m[1m${msg2}[0m")
	else()
		message(STATUS "{nany}  ${msg1}${msg2}")
	endif()
endmacro()


macro(MMESSAGE_MODULE msg)
	NMESSAGE_TITLE("[module] " "${msg}")
endmacro()


macro(NWARNING msg)
	if(UNIX)
		message(STATUS "[1;33m{nany}  [warning][0m ${msg}")
	else()
		message(STATUS "{nany}  [WARNING] ${msg}")
	endif()
endmacro()


macro(NERROR msg)
	if(UNIX)
		message(STATUS "[1;31m{nany}  [error][0m ${msg}")
	else()
		message(STATUS "{nany}  [ERROR] ${msg}")
	endif()
	set(NANY_ERROR_HAS_OCCURED  true)
endmacro()


macro(NFATAL msg)
	if(UNIX)
		message(FATAL_ERROR "[1;31m{nany}  [error][0m ${msg}")
	else()
		message(FATAL_ERROR "{nany}  [ERROR] ${msg}")
	endif()

	set(NANY_ERROR_HAS_OCCURED  true)
endmacro()


