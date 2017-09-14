/*
Copyright (C) 1996-1997 Id Software, Inc.
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

#pragma once

#include <list>
#include "cmd.hpp"

typedef struct cmd_function_s
{
	cmd_function_s(const char *aname, xcommand_t afunction, const char *adesc = "")
		: name(aname), function(afunction), desc(adesc){}
	~cmd_function_s() = default;
	
	const char *GetName() const {return name;}
	const char *GetDesc() const {return desc;}
	
	void Exec(const CCmdArgs &aArgs)
	{
		if(function)
			function(aArgs);
	};
	
	cmd_function_s *next{nullptr};
	
	const char *name{""};
	const char *desc{""};
	
	xcommand_t function{nullptr};
} cmd_function_t;

using tCmdList = std::list<cmd_function_t*>;

class CCmdList final
{
public:
	CCmdList();
	~CCmdList();
	
	/// Called by the init functions of other parts of the program to
	/// register commands and functions to call for them.
	/// The cmd_name is referenced later, so it should not be in temp memory
	bool Add(const char *cmd_name, xcommand_t function, const char *desc = "");
	
	///
	bool Remove(const char *cmd_name);
	
	///
	cmd_function_t *Find(const char *cmd_name) const;
	
	/// Used by the cvar code to check for cvar / command name overlap
	bool Exists(const char *cmd_name) const;
private:
	tCmdList mlstCmds; ///< possible commands to execute
};