project(IdTech2Red-game)

# Set default impl type
set(IDTECH2RED_GAME_DEFAULT_IMPL Blank)

# Detect the impl type
if(NOT IDTECH2RED_GAME_IMPL_TYPE)
    set(IDTECH2RED_GAME_IMPL_TYPE Default CACHE STRING
        "Choose the type engine module of implementation"
        FORCE
	)
    set_property(CACHE IDTECH2RED_GAME_IMPL_TYPE PROPERTY STRINGS
		Default
		Blank      # Null implementation (debug/dev)
		CTF
	)
endif()

if(${IDTECH2RED_GAME_IMPL_TYPE} STREQUAL Default)
	set(IDTECH2RED_GAME_IMPL_TYPE ${IDTECH2RED_GAME_DEFAULT_IMPL})
endif()

# Add the impl-specific sources
string(TOLOWER ${IDTECH2RED_GAME_IMPL_TYPE} IDTECH2RED_GAME_IMPL_TYPE)
add_subdirectory(${IDTECH2RED_GAME_IMPL_TYPE})