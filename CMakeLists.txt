cmake_minimum_required(VERSION 2.8)
project(idTech2-Red)

# Available options
option(IDTECH2RED_DEDICATED "Skip building of client-side modules" ON)

# Auto build number
file(READ "BuildNum.ini" IDTECH2RED_PREVBUILD LIMIT 4)
math(EXPR IDTECH2RED_BUILD "${IDTECH2RED_PREVBUILD} + 1")
#set(IDTECH2RED_PREVBUILD ${IDTECH2RED_BUILD} CACHE INTERNAL "Previous build number")
message(STATUS "Starting Build #" ${IDTECH2RED_BUILD})
file(WRITE "BuildNum.ini" ${IDTECH2RED_BUILD})

# Don't add the "lib" prefix for shared libs
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(CMAKE_SHARED_MODULE_PREFIX "")

# Set C++ code standard
set(CMAKE_CXX_STANDARD 14) #17 ?
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify output directory (for compiled binaries)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/../bin)

# Add include directories
include_directories(
	${CMAKE_SOURCE_DIR}/common
)

# Add subdirectories
add_subdirectory(dedicated)
add_subdirectory(engine)
add_subdirectory(network)
#add_subdirectory(qwfwd)
add_subdirectory(game)

if(NOT IDTECH2RED_DEDICATED)
	add_subdirectory(launcher)
	#add_subdirectory(render)
	add_subdirectory(input)
	add_subdirectory(sound)
	add_subdirectory(menu)
	add_subdirectory(clgame)
endif()