cmake_minimum_required(VERSION 2.8) 

PROJECT (SeverPlant)

set( SERVER_SRC
	src/main.cpp
	src/ServerPlant.cpp
	src/ServerPlant.h
)

set( CLIENT_SRC
	src/ClientPlant.cpp
)

set( CLIENT_INCLUDE
	src/ClientPlant.h
)

set( UTILS_SRC
	src/utils/Loader.cpp
	src/utils/ResponsePacket.cpp
	src/utils/network/TcpComm.cpp
	src/utils/network/UdpComm.cpp
	src/utils/network/ServerTCP.cpp
	src/utils/network/ClientTCP.cpp
	src/utils/SystemCall.cpp
	src/utils/security/Cryptography.cpp
	src/plant/Assembly.cpp
	src/plant/Part.cpp
)

set ( UTILS_INCLUDE
	src/utils/Loader.h
	src/utils/ResponsePacket.h
	src/utils/network/UdpComm.h
	src/utils/network/TcpComm.h
	src/utils/network/ServerTCP.h
	src/utils/network/ClientTCP.h
	src/utils/SystemCall.h
	src/utils/Queue.h
	src/utils/Runnable.h
	src/utils/util.h
	src/utils/security/Cryptography.h
	src/utils/serialization/json.h
	src/utils/PacketComm.h
	src/plant/Assembly.h
	src/plant/Part.h
)

if (WIN32)
    set(LIBSOCI "${CMAKE_CURRENT_SOURCE_DIR}/utils/SOCI/Windows")
endif (WIN32)

include_directories("${LIBSOCI}/include")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/utils/PostgreSQL/include")

if("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
	set(LIBSOCI_LIB "${LIBSOCI}/lib/x64")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/x64)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/x64)
else("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")
	set(LIBSOCI_LIB "${LIBSOCI}/lib/x86")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/x86)
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/x86)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/x86)
endif("${CMAKE_GENERATOR}" MATCHES "(Win64|IA64)")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CONFIGURATION_TYPES "Debug;Release")

find_package (Threads REQUIRED)

add_library( utilsCore ${UTILS_SRC} ${UTILS_INCLUDE})
if(WIN32)
	# Include winmm for timeGetTime function, wsock32 and ws2_32 for socket windows.
	target_link_libraries( utilsCore "${LIBSOCI_LIB}/$(Configuration)/soci_core_4_0.lib" "${LIBSOCI_LIB}/$(Configuration)/soci_postgresql_4_0.lib" wsock32 ws2_32 winmm)
else()
	target_link_libraries( utilsCore "${LIBSOCI_LIB}/${CMAKE_BUILD_TYPE}/soci_core_4_0.lib" "${LIBSOCI_LIB}/$(Configuration)/soci_postgresql_4_0.lib" )
endif()

add_library ( clientPlant ${CLIENT_SRC} ${CLIENT_INCLUDE} ${UTILS_INCLUDE} )
target_link_libraries( clientPlant utilsCore )

add_executable ( ServerPlant ${SERVER_SRC} ${UTILS_INCLUDE} )
target_link_libraries( ServerPlant utilsCore )

add_executable ( test_server src/test/test_server.cpp ${UTILS_INCLUDE} ${CLIENT_INCLUDE} )
target_link_libraries( test_server utilsCore clientPlant )

if(WIN32)
	string(REPLACE "/" "\\" LIBSOCI_DLL "${LIBSOCI_LIB}")
	string(REPLACE "/" "\\" TARGET_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$(Configuration)")
	add_custom_command(TARGET ServerPlant
					   POST_BUILD
					   COMMAND xcopy "${LIBSOCI_DLL}\\$(Configuration)\\*.dll" "${TARGET_DIR}" /D /K /Y
					   COMMENT "Copy DLLs ..."
					   VERBATIM
					   )
endif()