#include "commontypes.hpp"
#include "ClientGame.hpp"

C_EXPORT IClientGame *GetClientGame()
{
	static CClientGame ClientGame;
	return &ClientGame;
};