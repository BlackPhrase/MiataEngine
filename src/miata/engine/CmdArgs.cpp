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

#include "quakedef.hpp"
#include "CmdArgs.hpp"

static const char *cmd_null_string{""};

CCmdArgs::CCmdArgs(int argc, char **argv)
{
	//for(auto i = 0; i < argc; ++i)
		//mvArgs.emplace_back(argv[argc]); // TODO: fix
};

CCmdArgs::CCmdArgs(const char *args)
{
	if(!args || !*args)
		return; // throw?
	
	std::string str{args};
	std::string arg{""};
	
	auto pos{str.find_first_of(" ")};
	
	mvArgs.push_back(str.substr(0, pos));
	
	while(pos != std::string::npos && pos < str.size())
	{
		//printf("Current pos: %d\n", pos);
		//system("pause");
		
		arg = str.substr(pos);
		
		//printf("Arg: %s\n", arg.c_str());
		//system("pause");
		
		mvArgs.push_back(arg);
		pos = str.find(" ", pos + 1);
		
		//printf("Pos now is at: %d\n", pos);
		//system("pause");
	};
};

CCmdArgs::~CCmdArgs() = default;

/*
============
Cmd_Argv
============
*/
const char *CCmdArgs::GetByIndex(int id) const
{
	if(mvArgs.empty() || (unsigned)id >= mvArgs.size())
		return cmd_null_string;
	
	return mvArgs.at(id).c_str(); // mvArgs[id];
};

/*
================
Cmd_CheckParm

Returns the position (1 to argc-1) in the command's argument list
where the given parameter apears, or 0 if not present
================
*/

int CCmdArgs::CheckParm(const char *parm)
{
	if(!parm)
		Sys_Error("Cmd_CheckParm: NULL");

	for(int i = 1; i < GetCount(); ++i)
		if(!Q_strcasecmp(parm, GetByIndex(i)))
			return i;

	return 0;
};

/*
============
Cmd_Args

Returns a single string containing argv(1) to argv(argc()-1)
============
*/
const char *CCmdArgs::ToString() const
{
	static std::string sOutStr{""}; // TODO: fix
	
	for(auto It : mvArgs)
		if(!sOutStr.empty())
			sOutStr.append(" ").append(It);
		else
			sOutStr.append(It);
	
	return sOutStr.c_str();
};