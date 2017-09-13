#include <memory>
#include "quakedef.hpp"
#include "CvarList.hpp"
#include "IConVar.hpp"

CCvarList::CCvarList() = default;

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
	
	mlstCvars.push_back(variable);
	
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
IConVar *CCvarList::Find(const char *name)
{
	//printf("Trying to find the cvar %s...\n", name);
	
	for(auto It : mlstCvars)
		if(!Q_strcmp(It->GetName(), name))
			return It;
	
	//printf("not found!\n");
	return nullptr;
};