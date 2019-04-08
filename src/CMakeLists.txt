include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/common
	${CMAKE_CURRENT_SOURCE_DIR}/scitech/include
)

add_subdirectory(ctf)
add_subdirectory(game)

#add_subdirectory(ref_gl)
#add_subdirectory(ref_soft)

add_subdirectory(engine)

add_subdirectory(launcher)
add_subdirectory(dedicated)