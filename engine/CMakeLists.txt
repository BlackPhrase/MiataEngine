project(idTech2Red-engine)

list(APPEND CMAKE_CXX_FLAGS "-fpermissive -Wwrite-strings")

# Sources list
file(GLOB ENGINE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Add platform-specific sources
#add_subdirectory(platform)

# Build as module library
add_library(${PROJECT_NAME} MODULE ${ENGINE_SOURCES})

# Set custom properties
set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "engine"
)

if(NOT WIN32)
	target_link_libraries(${PROJECT_NAME} -ldl)
endif()