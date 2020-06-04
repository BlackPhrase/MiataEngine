/*
Copyright (C) 2017-2020 BlackPhrase

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

#include "commontypes.hpp"
#include "clgame/IClientGame.h"
#include "ClientGame.hpp"

namespace
{

CClientGame *gpClientGame{nullptr};

bool ClientGame_Init()
{
	return gpClientGame->Init();
};

void ClientGame_Shutdown()
{
	gpClientGame->Shutdown();
};

void ClientGame_Frame()
{
	gpClientGame->Frame();
};

void ClientGame_ParseServerMessage(int cmd, sizebuf_t &net_message)
{
	gpClientGame->ParseServerMessage(cmd, net_message);
};

void ClientGame_CreateMove(usercmd_t &cmd)
{
	gpClientGame->CreateMove(cmd);
};

clgame_export_t gClGame =
{
	CLGAME_API_VERSION,
	
	ClientGame_Init,
	ClientGame_Shutdown,
	
	ClientGame_Frame,
	
	ClientGame_ParseServerMessage,
	
	ClientGame_CreateMove,
};

}; // namespace

C_EXPORT clgame_export_t *GetClGameAPI(engine_api_t *apEngine)
{
	static CClientGame ClientGame(apEngine);
	gpClientGame = &ClientGame;
	return &gClGame;
};