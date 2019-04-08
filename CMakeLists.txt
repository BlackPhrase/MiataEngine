cmake_minimum_required(VERSION 3.8)
project(MiataEngine)

# Available options
option(MIATA_DEDICATED "Skip building of client-side modules" ON)

# Auto build number
file(READ "BuildNum.ini" MIATA_PREVBUILD LIMIT 4)
math(EXPR MIATA_BUILD "${MIATA_PREVBUILD} + 1")
#set(MIATA_PREVBUILD ${MIATA_BUILD} CACHE INTERNAL "Previous build number")
file(WRITE "BuildNum.ini" ${MIATA_BUILD})

message(STATUS "Starting Build #" ${MIATA_BUILD})

# Don't add the "lib" prefix for shared libs
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")

# Set C++ code standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify output directory (for compiled binaries)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../bin)

set(MIATA_KONBINILIB_PATH
	${CMAKE_SOURCE_DIR}/../konbinilib
	CACHE PATH
	"Path to konbinilib"
)

include_directories(${MIATA_KONBINILIB_PATH}/include)

# Add subdirectories
add_subdirectory(src)