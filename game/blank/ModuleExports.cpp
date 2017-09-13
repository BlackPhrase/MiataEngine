#include "commontypes.hpp"
#include "Game.hpp"

C_EXPORT IGame *GetGame()
{
	static CGame Game;
	return &Game;
};