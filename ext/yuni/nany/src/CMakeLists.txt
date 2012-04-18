cmake_minimum_required(VERSION 2.8)
project(nany)

include("cmake/message.cmake")
NMESSAGE("")
NMESSAGE_BOLD("Nany compiler")


# Package informations
set(NANY_URL_WEBSITE             "http://www.libyuni.org")
set(NANY_MAILING_LIST            "dev@libyuni.org")
# Version
set(NANY_HI                      0)
set(NANY_LO                      1)
set(NANY_VERSION                 "${NANY_HI}.${NANY_LO}")
set(NANY_VERSION_REV             0)
set(NANY_TARGET                  "debug") # Target : debug/release - see Target.cmake






# CMake policies
# --------------
# Preprocessor definition values are now escaped automatically
if(POLICY CMP0005)
	CMake_Policy(SET CMP0005 NEW)
endif()
if(POLICY CMP0011)
	CMake_Policy(SET CMP0011 NEW)
endif()
if(POLICY CMP0015)
	CMake_Policy(SET CMP0015 NEW)
endif()



# CMake special variables
# -----------------------
set(VS_KEYWORD                   "YuniVS${NANY_VERSION}")
set(VERSION                      "${NANY_VERSION}")
set(VS_SCC_PROJECTNAME           "Nany compiler")
set(PROJECT_LABEL                "Nany compiler")


# Target Detection
# ----------------
if ("${CMAKE_BUILD_TYPE}" STREQUAL "release" OR "${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
	set(NANY_TARGET "release")
else()
	set(NANY_TARGET "debug")
endif()


# ICC Detection
# -------------
# CMake does not handle ICC at all, mainly because ICC behaves like Visual Studio and
# it seems there is no reliable way to make the distinction
if (WIN32 OR WIN64)
	if (NOT MSVC AND NOT MINGW AND NOT CYGWIN)
		# Should behaves like Visual Studio, like ICC
		set(MSVC 1)
	endif()
endif()


# Common CXX flags
# ----------------
include("cmake/common-settings.cmake")


# Yuni
# ----
if (NOT EXISTS "../../src/build/${NANY_TARGET}/lib")
	NERROR("Please compile yuni first")
endif()
include_directories("../../src")
link_directories("../../src/build/${NANY_TARGET}/lib")
link_libraries(yuni-static-core)







# Tools - fix-parser
# ------------------
add_subdirectory(tools)


# Yuni - UUID
# -----------
link_libraries(yuni-static-uuid)
if (WIN32 OR WIN64)
	link_libraries(ole32)
	link_libraries(rpcrt4)
else()
	link_libraries(uuid)
endif()


add_executable(nany-bootstrap
	main.cpp
	parser/parser.cpp
	parser/parser.h
	typing/type.cpp
	parser/engine.c
	parser/engine.h
	parser/grammar.h

	ast/all.h
	ast/argumentlistnode.h
	ast/asexpressionnode.h
	ast/assignmentexpressionnode.h
	ast/attributedeclarationnode.h
	ast/classdeclarationnode.h
	ast/cppwritervisitor.cpp
	ast/cppwritervisitor.h
	ast/declarationlistnode.h
	ast/divideexpressionnode.h
	ast/dotexpressionnode.h
	ast/enumdeclarationnode.h
	ast/equalexpressionnode.h
	ast/expressionlistnode.h
	ast/forexpressionnode.h
	ast/functioncallnode.h
	ast/functiondeclarationnode.h
	ast/fwd.h
	ast/identifiernode.h
	ast/ifexpressionnode.h
	ast/inferiorequalexpressionnode.h
	ast/inferiorexpressionnode.h
	ast/isexpressionnode.h
	ast/literalnode.h
	ast/methoddeclarationnode.h
	ast/minusexpressionnode.h
	ast/modulusexpressionnode.h
	ast/multiplyexpressionnode.h
	ast/newexpressionnode.h
	ast/node.h
	ast/notequalexpressionnode.h
	ast/parallelexpressionnode.h
	ast/parameterlistnode.h
	ast/plusexpressionnode.h
	ast/programnode.h
	ast/returnexpressionnode.h
	ast/scopenode.h
	ast/shiftleftexpressionnode.h
	ast/shiftrightexpressionnode.h
	ast/simplevisitor.h
	ast/superiorequalexpressionnode.h
	ast/superiorexpressionnode.h
	ast/treeprintervisitor.h
	ast/typealiasnode.h
	ast/typeexpressionnode.h
	ast/typeofexpressionnode.h
	ast/typequalifierlistnode.h
	ast/unitdeclarationnode.h
	ast/vardeclarationnode.h
	ast/visibilityqualifiernode.h
	ast/visitor.h
	ast/whileexpressionnode.h
)








if(NANY_ERROR_HAS_OCCURED)
	message(STATUS "")
	message(STATUS "")
	if(UNIX)
		message(SEND_ERROR "[1;31mAn error has occured. Aborting. [0m")
	else()
		message(SEND_ERROR "An error has occured. Aborting.")
	endif()
endif()

# Empty message, for beauty :)
NMESSAGE("") # done

