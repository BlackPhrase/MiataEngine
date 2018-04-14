#include "commontypes.hpp"
#include "Game.hpp"

C_EXPORT IGame *GetGame(IEngineInterface *apEngineInterface)
{
	static CGame Game(apEngineInterface);
	return &Game;
};