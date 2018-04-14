# Add subdirectories

add_subdirectory(engine)
add_subdirectory(network)

if(NOT MIATA_DEDICATED)
	#add_subdirectory(render)
	add_subdirectory(video)
	add_subdirectory(input)
	add_subdirectory(sound)
	add_subdirectory(cdaudio)
endif()