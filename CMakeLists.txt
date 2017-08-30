cmake_minimum_required(VERSION 2.8)
project(idTech2-Red)

# Available options
option(IDTECH2RED_DEDICATED "Skip building of client-side modules" ON)

# Add include directories
include_directories(
	${CMAKE_SOURCE_DIR}/common
)

# Add subdirectories
add_subdirectory(dedicated)
add_subdirectory(engine)
add_subdirectory(network)
#add_subdirectory(qwfwd)

if(NOT IDTECH2RED_DEDICATED)
	add_subdirectory(launcher)
	add_subdirectory(render)
	add_subdirectory(input)
	add_subdirectory(sound)
	#add_subdirectory(menu)
	add_subdirectory(game)
endif()