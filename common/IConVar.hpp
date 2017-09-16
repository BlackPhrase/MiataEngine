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
/// @brief console variable interface

#pragma once

#include "commontypes.hpp"
#include "ICvarDispatcher.hpp"

/*
==========================================================

CVARS (console variables)

==========================================================
*/

enum class eConVarFlags : int
{
	Archive = BIT(0),   ///< set to cause it to be saved to vars.rc
	UserInfo = BIT(1),  ///< added to userinfo  when changed
	ServerInfo = BIT(2) ///< added to serverinfo when changed
	//Server = BIT(3) ///< notifies players when changed
};

//struct ICvarDispatcher;

struct IConVar
{
	///
	virtual void SetDispatcher(ICvarDispatcher *dispatcher) = 0;
	
	///
	virtual void Release() = 0;
	
	///
	virtual const char *GetName() const = 0;
	
	///
	virtual const char *GetDesc() const = 0;
	
	///
	virtual int GetFlags() const = 0;
	
	///
	virtual void SetString(const char *value) = 0;
	
	///
	virtual void SetInt(int value) = 0;
	
	///
	virtual void SetFloat(float value) = 0;
	
	///
	virtual void SetBool(bool value) = 0;
	
	///
	virtual const char *GetString() const = 0;
	
	///
	virtual int GetInt() const = 0;
	
	///
	virtual float GetFloat() const = 0;
	
	///
	virtual bool GetBool() const = 0;
protected:
	virtual ~IConVar() = default;
};