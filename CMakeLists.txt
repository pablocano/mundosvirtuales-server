cmake_minimum_required(VERSION 2.8) 

PROJECT (SeverPlant)

set( SERVER_SRC
	src/main.cpp
	src/ServerPlant.cpp
	src/utils/communication/ResponsePacketServer.cpp
	src/utils/db/DBAdapterSOCI.cpp
)

set( SERVER_INCLUDE
	src/ServerPlant.h
	src/utils/communication/ResponsePacketServer.h
	src/utils/db/DBAdapterSOCI.h
)

set( DB_SRC
	src/utils/db/DBAdapter.cpp
	src/utils/db/FieldData.cpp
	src/utils/db/RegisterValue.cpp
	src/utils/db/Row.cpp
	src/utils/db/Rows.cpp
)

set( DB_INCLUDE
	src/utils/db/DBAdapter.h
	src/utils/db/FieldData.h
	src/utils/db/RegisterValue.h
	src/utils/db/Row.h
	src/utils/db/Rows.h
)

set( CLIENT_SRC
	src/ClientPlant.cpp
	src/utils/communication/ResponsePacketClient.cpp
)

set( CLIENT_INCLUDE
	src/ClientPlant.h
	src/utils/communication/ResponsePacketClient.h
)

set( UTILS_SRC
	src/plant/Assembly.cpp
	src/plant/InfoAssembly.cpp
	src/plant/ModelAssembly.cpp
	src/plant/ModelControl.cpp
	src/plant/StockPlant.cpp
	src/utils/AssemblyUtils.cpp
	src/utils/Position.cpp
	src/utils/Logger/Logger.cpp
	src/utils/administration/Group.cpp
	src/utils/administration/Permission.cpp
	src/utils/administration/Roles.cpp
	src/utils/administration/User.cpp
	src/utils/communication/ResponsePacket.cpp
	src/utils/db/ObjectDB.cpp
	src/utils/network/TcpComm.cpp
	src/utils/network/UdpComm.cpp
	src/utils/network/ServerTCP.cpp
	src/utils/network/ClientTCP.cpp
	src/utils/platform/SystemCall.cpp
	src/utils/security/Cryptography.cpp
)

set ( UTILS_INCLUDE
	src/plant/Assembly.h
	src/plant/InfoAssembly.h
	src/plant/ModelAssembly.h
	src/plant/ModelControl.h
	src/plant/PlantException.h
	src/plant/StockPlant.h
	src/plant/sensors/Sensor.h
	src/plant/sensors/SensorException.h
	src/utils/AssemblyUtils.h
	src/utils/Exception.h
	src/utils/Logger/Logger.h
	src/utils/Logger/LoggerException.h
	src/utils/Position.h
	src/utils/Queue.h
	src/utils/util.h
	src/utils/administration/AdministrationException.h
	src/utils/administration/Group.h
	src/utils/administration/Permission.h
	src/utils/administration/Roles.h
	src/utils/administration/User.h
	src/utils/communication/CommunicationException.h
	src/utils/communication/ResponsePacket.h
	src/utils/communication/PacketComm.h
	src/utils/concurrency/Concurrency.h
	src/utils/concurrency/ConcurrencyException.h
	src/utils/concurrency/Runnable.h
	src/utils/db/ObjectDB.h
	src/utils/network/NetworkException.h
	src/utils/network/ClientTCP.h
	src/utils/network/ServerTCP.h
	src/utils/network/TcpComm.h
	src/utils/network/UdpComm.h
	src/utils/platform/PlatformException.h
	src/utils/platform/SystemCall.h
	src/utils/security/SecurityException.h
	src/utils/security/Cryptography.h
	src/utils/serialization/json.h
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

add_library( utilsCore ${UTILS_SRC} ${DB_SRC} ${UTILS_INCLUDE} ${DB_INCLUDE} )
if(WIN32)
	# Include winmm for timeGetTime function, wsock32 and ws2_32 for socket windows.
	target_link_libraries( utilsCore "${LIBSOCI_LIB}/$(Configuration)/soci_core_4_0.lib" "${LIBSOCI_LIB}/$(Configuration)/soci_postgresql_4_0.lib" wsock32 ws2_32 winmm)
else()
	target_link_libraries( utilsCore "${LIBSOCI_LIB}/${CMAKE_BUILD_TYPE}/soci_core_4_0.lib" "${LIBSOCI_LIB}/$(Configuration)/soci_postgresql_4_0.lib" )
endif()

add_library ( clientPlant ${CLIENT_SRC} ${CLIENT_INCLUDE} ${UTILS_INCLUDE} )
target_link_libraries( clientPlant utilsCore )

add_executable ( ServerPlant ${SERVER_SRC} ${SERVER_INCLUDE} ${UTILS_INCLUDE} )
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