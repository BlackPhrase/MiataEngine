project(Miata-engine)

add_library(${PROJECT_NAME} MODULE)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/null PROJECT_SOURCES_PLATFORM)

target_sources(${PROJECT_NAME}
	PRIVATE ${PROJECT_SOURCES}
	PRIVATE ${PROJECT_SOURCES_PLATFORM}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME "engine"
)