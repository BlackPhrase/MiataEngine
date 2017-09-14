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

// cvar.h

#pragma once

#include <string>
#include "quakedef.hpp"
#include "IConVar.hpp"

class cvar_t final : public IConVar
{
public:
	cvar_t(const char *name, const char *defvalue, int flags = 0, const char *desc = "")
		: m_name(name), string(defvalue), m_flags(flags), m_desc(desc){}
	
	void Release(){delete this;}
	
	const char *GetName() const {return m_name;}
	const char *GetDesc() const {return m_desc;}
	
	int GetFlags() const {return m_flags;}
	
	void SetString(const char *value){string = value;}
	void SetInt(int value){string = std::to_string(value);}
	void SetFloat(float value){string = std::to_string(value);}
	void SetBool(bool value){string = std::to_string(value);}
	
	const char *GetString() const {return string.c_str();}
	int GetInt() const {return (int)GetFloat();}
	float GetFloat() const {return 0.0f;}
	bool GetBool() const {return false;}
private:
	const char *m_name{""};
	std::string string{""};
	const char *m_desc{""};

	bool archive /*info in qw*/{false}; // set to true to cause it to be saved to vars.rc
	bool server{false};                 // notifies players when changed
	
	int m_flags{0};

	cvar_t *next{nullptr};
};

const char *Cvar_CompleteVariable(const char *partial);
// attempts to match a partial variable name for command line completion
// returns NULL if nothing fits

//void Cvar_WriteVariables(FILE *f);
// Writes lines containing "set variable value" for all variables
// with the archive flag set to true.