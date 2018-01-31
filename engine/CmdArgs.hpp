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
/// @brief command (line) arguments

#pragma once

#include <string>
#include <vector>
#include "ICmdArgs.hpp"

using tStringVec = std::vector<std::string>;

class CCmdArgs final : public ICmdArgs
{
public:
	CCmdArgs(int argc, char **argv);
	CCmdArgs(const char *args);
	~CCmdArgs();
	
	// The functions that execute commands get their parameters with these
	// functions. Cmd_Argv () will return an empty string, not a NULL
	// if arg > argc, so string operations are allways safe.
	
	/// Takes a null terminated string
	/// Does not need to be /n terminated
	/// Breaks the string up into arg tokens
	//void TokenizeString(const char *text);
	
	const char *GetByIndex(int id) const override;
	
	/// Returns the position (1 to argc-1) in the command's argument list
	/// where the given parameter apears, or 0 if not present
	//int CheckParm(const char *parm);
	
	int GetCount() const override {return mvArgs.size();}
	
	const char *ToString() const override;
private:
	tStringVec mvArgs;
	//static char *cmd_args{nullptr};
};