project(idTech2Red-clgame)

# Sources list
file(GLOB PROJECT_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Build as module library
add_library(${PROJECT_NAME} MODULE ${PROJECT_SOURCES})

# Set custom properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "clgame"
)