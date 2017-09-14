/*
Copyright (C) 2017 Sh1ft0x0EF

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
/// @brief command execution component

#pragma once

#include "cmd.hpp"

class CCvarList;
class CCmdList;
class CCvarManager;
class CCmdArgs;

class CCmdExecutor final
{
public:
	CCmdExecutor(CCvarList *apCvarList, CCmdList *apCmdList, CCvarManager *apCvarManager);
	~CCmdExecutor();
	
	// Parses a single line of text into arguments and tries to execute it.
	// The text can come from the command buffer, a remote client, or stdin.
	void ExecString(const char *text, cmd_source_t src);
private:
	// Called by Cmd_ExecuteString when Cmd_Argv(0) doesn't match a known
	// command. Returns true if the command was a variable reference that
	// was handled. (print or change)
	bool HandleCvarCommand(const CCmdArgs &aArgs);
	
	CCvarList *mpCvarList{nullptr};
	CCmdList *mpCmdList{nullptr};
	CCvarManager *mpCvarManager{nullptr};
};