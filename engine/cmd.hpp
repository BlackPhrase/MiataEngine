/*
Copyright (C) 1996-1997 Id Software, Inc.
Copyright (C) 2017-2018 BlackPhrase

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
/// @brief command execution

#pragma once

//===========================================================================

/*

Command execution takes a null terminated string, breaks it into tokens,
then searches for a command or variable that matches the first token.

Commands can come from three sources, but the handler functions may choose
to dissallow the action or forward it to a remote server if the source is
not apropriate.

*/

struct ICmdArgs;

enum cmd_source_t
{
	src_client, // came in over a net connection as a clc_stringcmd
	// host_client will be valid during this state.
	src_command // from the command buffer
};

extern cmd_source_t cmd_source;

void Cmd_Init();

char *Cmd_CompleteCommand(const char *partial);
// attempts to match a partial command for automatic command line completion
// returns NULL if nothing fits

void Cmd_ForwardToServer(const ICmdArgs &aArgs);
// adds the current command line as a clc_stringcmd to the client message.
// things like godmode, noclip, etc, are commands directed to the server,
// so when they are typed in at the console, they will need to be forwarded.