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
	
	void SetDispatcher(ICvarDispatcher *dispatcher) override {mpDispatcher = dispatcher;}
	
	void Release() override {delete this;}
	
	const char *GetName() const override {return m_name;}
	const char *GetDesc() const override {return m_desc;}
	
	int GetFlags() const override {return m_flags;}
	
	void SetString(const char *value) override
	{
		std::string oldval{string};
		
		string = value;
		
		if(mpDispatcher)
			mpDispatcher->BroadcastCvarChange(this, oldval.c_str());
	};
	
	void SetInt(int value) override {string = std::to_string(value);}
	void SetFloat(float value) override {string = std::to_string(value);}
	void SetBool(bool value) override {string = std::to_string(value);}
	
	const char *GetString() const override {return string.c_str();}
	int GetInt() const override {return (int)GetFloat();}
	float GetFloat() const override {return 0.0f;}
	bool GetBool() const override {return false;}
private:
	std::string string{""};
	
	ICvarDispatcher *mpDispatcher{nullptr};
	cvar_t *next{nullptr};
	
	const char *m_name{""};
	const char *m_desc{""};
	
	int m_flags{0};
};