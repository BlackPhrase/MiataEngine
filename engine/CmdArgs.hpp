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

#pragma once

#include <string>
#include <vector>

using tStringVec = std::vector<std::string>;

class CCmdArgs final
{
public:
	CCmdArgs(int argc, char **argv);
	CCmdArgs(const char *args);
	~CCmdArgs();
	
	// The functions that execute commands get their parameters with these
	// functions.
	
	/// Will return an empty string, not a nullptr
	/// if arg > argc, so string operations are always safe
	const char *GetByIndex(int id) const;
	
	///
	int GetCount() const {return mvArgs.size();}
	
	///
	const char *ToString() const;
private:
	tStringVec mvArgs;
	//static char *cmd_args{nullptr};
};