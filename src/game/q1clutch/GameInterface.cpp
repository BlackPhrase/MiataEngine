/*
Copyright (C) 2020 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file

#include "game/IGame.h"
#include "Game.hpp"

namespace
{

CGame *gpGame{nullptr};

bool Game_Init()
{
	return gpGame->Init();
};

void Game_Shutdown()
{
	gpGame->Shutdown();
};

void Game_Frame(float afTimeStep)
{
	gpGame->Frame(afTimeStep);
};

bool Game_ParseServerMessage(int anClientID, const sizebuf_t &net_message)
{
	return gpGame->ParseClientMessage(anClientID, net_message);
};

clgame_export_t gClGame =
{
	GAME_API_VERSION,
	
	Game_Init,
	Game_Shutdown,
	
	Game_Frame,
	
	Game_ParseClientMessage,
};

}; // namespace

C_EXPORT game_export_t *GetGameAPI(engine_api_t *apEngine)
{
	static CGame Game(apEngine);
	gpGame = &Game;
	return &Game;
};