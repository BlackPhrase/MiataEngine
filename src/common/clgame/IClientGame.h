/*
Copyright (C) 2017-2019 BlackPhrase

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
/// @brief client-side game module interface

#pragma once

#include "engine/engine_api.h"

#define	CLGAME_API_VERSION 1

//
// functions exported by the client game subsystem
//
struct clgame_export_t
{
	int apiversion{CLGAME_API_VERSION};
	
	///
	bool (*Init)();

	///
	void (*Shutdown)();

	///
	void (*Frame)();
	
	///
	bool (*ParseServerMessage)(int cmd, sizebuf_t &net_message);
	
	///
	void (*BaseMove)(usercmd_t *cmd);
};

clgame_export_t *GetClGameAPI(engine_api_t *apEngine);