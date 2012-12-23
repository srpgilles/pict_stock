
YMESSAGE_MODULE("Network  (EXPERIMENTAL)")

LIBYUNI_CONFIG_LIB("both" "net"        "yuni-static-net")



Set(SRC_NET_PROTO
	net/net.h
	net/errors.h
	net/port.h
	net/port.hxx
	net/hostaddressport.h
	net/hostaddressport.hxx
	net/protocol.h
	net/protocol.cpp

	net/message/queueservice.h
	net/message/queueservice.cpp
	net/message/queueservice.hxx
	net/message/transport.h
	net/message/transport/transport.h
	net/message/transport/transport.hxx
	net/message/worker.h
	net/message/worker.hxx
	net/message/worker.cpp
)
source_group("Network\\Protocol" FILES ${SRC_NET_PROTO})


Set(SRC_NET_MSG
	net/message/queueservice.h
	net/message/queueservice.cpp
	net/message/queueservice.hxx
)
source_group("Network\\Message" FILES ${SRC_NET_MSG})


add_Library(yuni-static-net STATIC ${SRC_NET_PROTO} ${SRC_NET_MSG})

# Setting output path
SET_TARGET_PROPERTIES(yuni-static-net PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${YUNI_OUTPUT_DIRECTORY}/lib")

# Installation
install(TARGETS yuni-static-net ARCHIVE DESTINATION lib/${YUNI_VERSIONED_INST_PATH})

# Install net-related headers
install(
	DIRECTORY net
	DESTINATION include/${YUNI_VERSIONED_INST_PATH}/yuni
	FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hxx"
	PATTERN ".svn" EXCLUDE
	PATTERN "CMakeFiles" EXCLUDE
	PATTERN "cmake" EXCLUDE
)
