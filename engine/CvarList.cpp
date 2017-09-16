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
/// @brief dynamic variable tracking

#include <memory>
#include "quakedef.hpp"
#include "CvarList.hpp"
#include "IConVar.hpp"

CCvarList::CCvarList(ICvarDispatcher *apCvarDispatcher)
	: mpCvarDispatcher(apCvarDispatcher){}

CCvarList::~CCvarList()
{
	for(auto It : mlstCvars)
		It->Release();
	
	mlstCvars.clear();
};

/*
============
Cvar_RegisterVariable

Adds a freestanding variable to the variable list.
============
*/
bool CCvarList::Register(IConVar *variable)
{
	// first check to see if it has already been defined
	if(Find(variable->GetName()))
	{
		//Con_Printf("Can't register variable %s, already defined\n", variable->GetName());
		//printf("Prevent double registration of cvar %s\n", variable->GetString());
		return false;
	};
	
	//printf("Got a new cvar %s!\n", variable->GetName());
	
	//try
	{
		if(mpCvarDispatcher)
			variable->SetDispatcher(mpCvarDispatcher);
		
		mlstCvars.push_back(variable);
	}
	//catch(...)
	{
		//return false;
	};
	
	// link the variable in
	//variable->next = cvar_vars;
	//cvar_vars = variable;
	
	return true;
};

IConVar *CCvarList::Create(const char *name, const char *defvalue, int flags, const char *desc)
{
	std::unique_ptr<cvar_t> pNewVar = std::make_unique<cvar_t>(name, defvalue, flags, desc);
	
	//printf("CvarList::Create : %s %s %d %s\n", name, defvalue, flags, desc);
	//printf("CvarList::Create : %s %s %d %s\n", pNewVar->GetName(), pNewVar->GetString(), pNewVar->GetFlags(), pNewVar->GetDesc());
	
	if(!Register(pNewVar.get()))
		return nullptr;
	
	return pNewVar.release();
};

/*
============
Cvar_FindVar
============
*/
IConVar *CCvarList::Find(const char *name) const
{
	//printf("Trying to find the cvar %s...\n", name);
	
	for(auto It : mlstCvars)
		if(!Q_strcmp(It->GetName(), name))
			return It;
	
	//printf("not found!\n");
	return nullptr;
};

IConVar *CCvarList::FindPartial(const char *name, int length) const
{
	for(auto It : mlstCvars)
		if(!Q_strncmp(It->GetName(), name, length))
			return It;
	
	return nullptr;
};