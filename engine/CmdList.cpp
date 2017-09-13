#include "quakedef.hpp"
#include "CmdList.hpp"
#include "CmdArgs.hpp"

CCmdList::CCmdList() = default;

CCmdList::~CCmdList()
{
	for(auto It : mlstCmds)
		delete It;
	
	mlstCmds.clear();
};

/*
============
Cmd_AddCommand
============
*/
bool CCmdList::Add(const char *cmd_name, xcommand_t function, const char *desc)
{
	// because hunk allocation would get stomped
	//if(host_initialized)
		//Sys_Error("Cmd_AddCommand after host_initialized");

	// fail if the command is a variable name
	//if(Cvar_VariableString(cmd_name)[0])
	{
		//Con_Printf("Cmd_AddCommand: %s already defined as a var\n", cmd_name);
		//return false;
	};

	// fail if the command already exists
	if(Exists(cmd_name))
	{
		//Con_Printf("Cmd_AddCommand: %s already defined\n", cmd_name);
		return false;
	};

	//cmd_function_s *cmd = Hunk_Alloc(sizeof(cmd_function_t));
	cmd_function_t *cmd = new cmd_function_t(cmd_name, function, desc);
	
	mlstCmds.push_back(cmd);
	
	//mlstCmds.emplace_back(cmd_name, function, desc);
	
	//cmd->next = cmd_functions;
	//cmd_functions = cmd;
	return true;
};

bool CCmdList::Remove(const char *cmd_name)
{
	cmd_function_t *pCmd{Find(cmd_name)};
	
	if(pCmd)
	{
		delete pCmd;
		mlstCmds.remove(pCmd);
		return true;
	};
	
	return false;
};

cmd_function_t *CCmdList::Find(const char *cmd_name) const
{
	for(auto It : mlstCmds)
		if(!Q_strcmp(It->GetName(), cmd_name)) // Q_strcasecmp
			return It;
	
	return nullptr;
};

/*
============
Cmd_Exists
============
*/
bool CCmdList::Exists(const char *cmd_name) const
{
	if(Find(cmd_name))
		return true;

	return false;
};