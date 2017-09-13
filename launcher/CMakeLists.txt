project(IdTech2Red-launcher)

file(GLOB PROJECT_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(${PROJECT_NAME} WIN32 ${PROJECT_SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "launcher"
)

if(NOT WIN32)
	target_link_libraries(${PROJECT_NAME} -ldl)
endif()