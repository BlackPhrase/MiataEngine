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

/// @file

#include "quakedef.hpp"
#include "CmdExecutor.hpp"
#include "CvarList.hpp"
#include "CmdList.hpp"
#include "CvarController.hpp"
#include "CmdArgs.hpp"

/*
=============================================================================

					COMMAND EXECUTION

=============================================================================
*/

cmd_source_t cmd_source;

CCmdExecutor::CCmdExecutor(CCvarList *apCvarList, CCmdList *apCmdList, CCvarController *apCvarController)
	: mpCvarList(apCvarList), mpCmdList(apCmdList), mpCvarController(apCvarController){}
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
	
	mpCvarController->SetCvarString(pVar->GetName(), aArgs.GetByIndex(1)); // TODO: fix
	return true;
};