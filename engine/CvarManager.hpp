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

#include <list>
#include "ICvarManager.hpp"

/*
cvar_t variables are used to hold scalar or string variables that can be changed or displayed at the console or prog code as well as accessed directly
in C code.

it is sufficient to initialize a cvar_t with just the first two fields, or
you can add a ,true flag for variables that you want saved to the configuration
file when the game is quit:

cvar_t	r_draworder = {"r_draworder","1"};
cvar_t	scr_screensize = {"screensize","1",true};

Cvars must be registered before use, or they will have a 0 value instead of the float interpretation of the string.  Generally, all cvar_t declarations should be registered in the apropriate init function before any console commands are executed:
Cvar_RegisterVariable (&host_framerate);

C code usually just references a cvar in place:
if ( r_draworder.value )

It could optionally ask for the value to be looked up for a string name:
if (Cvar_VariableValue ("r_draworder"))

The user can access cvars from the console in two ways:
r_draworder			prints the current value
r_draworder 0		sets the current value to 0
Cvars are restricted from having the same names as commands to keep this
interface from being ambiguous.
*/

struct IConVar;
class CCvarList;
class CCmdList;

struct ICvarChangeListener
{
	virtual void OnCvarChange(IConVar *apCvar, const char *asOldValue) = 0;
	
	virtual void Release(){delete this;}
protected:
	virtual ~ICvarChangeListener() = default;
};

using tCvarChangeListenerList = std::list<ICvarChangeListener*>;

class CCvarManager final : public ICvarManager
{
public:
	CCvarManager(CCvarList *apCvarList, CCmdList *apCmdList);
	~CCvarManager();
	
	///
	void AddListener(ICvarChangeListener *apListener) override;
	
	///
	void RemoveListener(ICvarChangeListener *apListener) override;
	
	///
	void BroadcastCvarChange(IConVar *apVar, const char *asOldValue) override;
	
	///
	bool RegisterCvar(IConVar *apVar) override;
	
	///
	const char *CompleteVariable(const char *partial);
	
	/// Equivelant to "<name> <variable>" typed at the console
	void SetCvarString(const char *var_name, const char *value) override;

	/// Expands value to a string and calls Cvar_Set
	void SetCvarFloat(const char *var_name, float value) override;
	
	/// Returns an empty string if not defined
	const char *GetCvarString(const char *var_name, const char *defval) override;
	
	/// Returns 0 if not defined or non numeric
	float GetCvarFloat(const char *var_name, float defval) override;
private:
	static constexpr auto cvar_null_string{""};
	
	tCvarChangeListenerList mlstListeners;
	
	CCvarList *mpCvarList{nullptr};
	CCmdList *mpCmdList{nullptr};
};