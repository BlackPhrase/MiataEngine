# Add include directories
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/common
)

# Add subdirectories

add_subdirectory(miata)

add_subdirectory(dedicated)
add_subdirectory(game)

if(NOT MIATA_DEDICATED)
	add_subdirectory(launcher)
	add_subdirectory(clgame)
	add_subdirectory(menu)
endif()

#add_subdirectory(qwfwd)