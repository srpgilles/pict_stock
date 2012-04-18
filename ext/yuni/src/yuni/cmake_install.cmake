# Install script for directory: /home/sebastien/Programmation/yuni/src/yuni

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "debug")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "yuni-core")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/yuni/0.2" TYPE STATIC_LIBRARY FILES "/home/sebastien/Programmation/yuni/src/build/debug/lib/libyuni-static-core.a")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "yuni-core")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "yuni-core")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/yuni/0.2/yuni" TYPE DIRECTORY FILES
    "/home/sebastien/Programmation/yuni/src/yuni/core"
    "/home/sebastien/Programmation/yuni/src/yuni/job"
    "/home/sebastien/Programmation/yuni/src/yuni/thread"
    "/home/sebastien/Programmation/yuni/src/yuni/io"
    FILES_MATCHING REGEX "/[^/]*\\.h$" REGEX "/[^/]*\\.hxx$" REGEX "/\\.svn$" EXCLUDE REGEX "/CMakeFiles$" EXCLUDE REGEX "/cmake$" EXCLUDE)
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "yuni-core")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "yuni-core")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/yuni/0.2/yuni" TYPE FILE FILES
    "/home/sebastien/Programmation/yuni/src/yuni/Doxygen.txt"
    "/home/sebastien/Programmation/yuni/src/yuni/doxygen.h"
    "/home/sebastien/Programmation/yuni/src/yuni/platform.h"
    "/home/sebastien/Programmation/yuni/src/yuni/yuni.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "yuni-core")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "yuni-core")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/yuni/0.2" TYPE FILE RENAME "yuni.version" FILES "/home/sebastien/Programmation/yuni/src/yuni/yuni.version")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "yuni-core")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "yuni-core")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/yuni/0.2" TYPE FILE RENAME "yuni.config.gcc" FILES "/home/sebastien/Programmation/yuni/src/yuni/yuni.config.gcc.target")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "yuni-core")

