include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/common
	${CMAKE_CURRENT_SOURCE_DIR}/scitech/include
)

#add_subdirectory(mathlib)
#add_subdirectory(qstdlib)

add_subdirectory(game)

add_subdirectory(clgame) # broken
add_subdirectory(render) # broken

add_subdirectory(engine) # broken
add_subdirectory(newframe)

add_subdirectory(launcher)
add_subdirectory(dedicated)

#add_subdirectory(qwfwd)

#add_subdirectory(utils)