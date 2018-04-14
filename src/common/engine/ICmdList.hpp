/*
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
/// @brief command list interface

#pragma once

#include "ICmdArgs.hpp"

using pfnCmdCallback = void (*)(const ICmdArgs &aArgs);

struct ICmdList
{
	/// Called by the init functions of other parts of the program to
	/// register commands and functions to call for them.
	/// The cmd_name is referenced later, so it should not be in temp memory
	virtual bool Add(const char *cmd_name, pfnCmdCallback function, const char *desc = "") = 0;
	
	/// Used by the cvar code to check for cvar / command name overlap
	virtual bool Exists(const char *cmd_name) const = 0;
};