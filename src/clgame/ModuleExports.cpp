/// @file

#include "commontypes.hpp"
#include "ClientGame.hpp"

C_EXPORT IClientGame *GetClientGame(IEngineInterface *apEngineInterface)
{
	static CClientGame ClientGame(apEngineInterface);
	return &ClientGame;
};