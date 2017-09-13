/// @file

#include "quakedef.hpp"
#include "CmdExecutor.hpp"
#include "CvarList.hpp"
#include "CmdList.hpp"
#include "CvarManager.hpp"
#include "CmdArgs.hpp"

/*
=============================================================================

					COMMAND EXECUTION

=============================================================================
*/

cmd_source_t cmd_source;

CCmdExecutor::CCmdExecutor(CCvarList *apCvarList, CCmdList *apCmdList, CCvarManager *apCvarManager)
	: mpCvarList(apCvarList), mpCmdList(apCmdList), mpCvarManager(apCvarManager){}
CCmdExecutor::~CCmdExecutor() = default;

/*
============
Cmd_ExecuteString

A complete command line has been parsed, so try to execute it
FIXME: lookupnoadd the token to speed search?
============
*/
void CCmdExecutor::ExecString(const char *text, cmd_source_t src)
{
	cmd_source = src;
	
	//Cmd_TokenizeString(text);
	CCmdArgs Args(text);
	
	//printf("Got a cmd to exec: %s\n", Args.ToString());
	
	// execute the command line
	if(!Args.GetCount())
		return; // no tokens
	
	// check functions
	cmd_function_t *pCmd{mpCmdList->Find(Args.GetByIndex(0))};
	
	if(pCmd)
	{
		pCmd->Exec(Args);
		return;
	};

	// check alias
	//for(cmdalias_t *a = cmd_alias; a; a = a->next)
	{
		//if(!Q_strcasecmp(Args.GetByIndex(0), a->name))
		{
			//Cbuf_InsertText(a->value);
			//return;
		};
	};
	
	// check cvars
	if(!HandleCvarCommand(Args))
	{
		//Con_Printf("Unknown command \"%s\"\n", Args.GetByIndex(0));
		printf("Unknown command \"%s\"\n", Args.GetByIndex(0));
		return;
	};
};

/*
============
Cvar_Command

Handles variable inspection and changing from the console
============
*/
bool CCmdExecutor::HandleCvarCommand(const CCmdArgs &aArgs)
{
	// check variables
	IConVar *pVar{mpCvarList->Find(aArgs.GetByIndex(0))};
	
	if(!pVar)
	{
		//printf("Can't find v... (%s)\n", aArgs.GetByIndex(0));
		return false;
	};
	
	// perform a variable print or set
	if(aArgs.GetCount() == 1)
	{
		//Con_Printf("\"%s\" is \"%s\"\n", pVar->GetName(), pVar->GetString());
		printf("\"%s\" is \"%s\"\n", pVar->GetName(), pVar->GetString());
		return true;
	};
	
	mpCvarManager->SetCvarString(pVar->GetName(), aArgs.GetByIndex(1)); // TODO: fix
	return true;
};