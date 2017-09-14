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

#include "quakedef.hpp"
#include "CvarManager.hpp"
#include "CvarList.hpp"
#include "CmdList.hpp"

CCvarManager::CCvarManager(CCvarList *apCvarList, CCmdList *apCmdList) : mpCvarList(apCvarList), mpCmdList(apCmdList){}

CCvarManager::~CCvarManager()
{
	for(auto It : mlstListeners)
		It->Release();
	
	mlstListeners.clear();
};

void CCvarManager::AddListener(ICvarChangeListener *apListener)
{
	mlstListeners.push_back(apListener);
};

void CCvarManager::RemoveListener(ICvarChangeListener *apListener)
{
	//mlstListeners.erase();
	// TODO
};

void CCvarManager::BroadcastCvarChange(IConVar *apVar, const char *asOldValue)
{
	//for(auto It : mlstListeners)
		//It->OnCvarChange(apVar, asOldValue);
};

bool CCvarManager::RegisterCvar(IConVar *apVar)
{
	// check for overlap with a command
	if(mpCmdList->Exists(apVar->GetName()))
	{
		//Con_Printf("Cvar_RegisterVariable: %s is a command\n", variable->GetName());
		return false;
	};
	
	return mpCvarList->Register(apVar);
};

/*
============
Cvar_CompleteVariable
============
*/
const char *CCvarManager::CompleteVariable(const char *partial)
{
	auto len{Q_strlen(partial)};

	if(!len)
		return nullptr;

	auto pCvar{mpCvarList->FindPartial(partial, len)};
	
	if(pCvar)
		return pCvar->GetName();

	return nullptr;
};

/*
============
Cvar_Set
============
*/
void CCvarManager::SetCvarString(const char *var_name, const char *value)
{
	auto *var{mpCvarList->Find(var_name)};
	
	if(!var)
	{
		// there is an error in C code if this happens
		//Con_Printf("Cvar_Set: variable %s not found\n", var_name);
		return;
	};

	bool bChanged{Q_strcmp(var->GetString(), value)};
	
	var->SetString(value);
	
	if(var->GetFlags() & (int)eConVarFlags::ServerInfo && bChanged)
	{
		//if(sv.active)
			//SV_BroadcastPrintf("\"%s\" changed to \"%s\"\n", var->name, var->string);
	};
};

/*
============
Cvar_SetValue
============
*/
void CCvarManager::SetCvarFloat(const char *var_name, float value)
{
	char val[32]{};

	sprintf(val, "%f", value);
	SetCvarString(var_name, val);
};

/*
============
Cvar_VariableString
============
*/
const char *CCvarManager::GetCvarString(const char *var_name, const char *defval)
{
	auto *pVar{mpCvarList->Find(var_name)};
	
	if(!pVar)
		return defval;
	
	return pVar->GetString();
};

/*
============
Cvar_VariableValue
============
*/
float CCvarManager::GetCvarFloat(const char *var_name, float defval)
{
	auto *pVar{mpCvarList->Find(var_name)};
	
	if(!pVar)
		return defval;
	
	return pVar->GetFloat();
};