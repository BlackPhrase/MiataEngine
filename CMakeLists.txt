cmake_minimum_required(VERSION 2.8)
project(idTech2-Red)

# Add include directories
include_directories(
	${CMAKE_SOURCE_DIR}/common
)

# Add subdirectories
add_subdirectory(dedicated)
add_subdirectory(launcher)
add_subdirectory(engine)
add_subdirectory(gas2masm)
add_subdirectory(qwfwd)