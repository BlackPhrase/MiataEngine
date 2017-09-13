/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
// cmd.c -- script command processing module

#include "quakedef.hpp"

void Cmd_ForwardToServer();

#define MAX_ALIAS_NAME 32

typedef struct cmdalias_s
{
	struct cmdalias_s *next;
	char name[MAX_ALIAS_NAME];
	char *value;
} cmdalias_t;

cmdalias_t *cmd_alias;

cvar_t cl_warncmd = { "cl_warncmd", "0" };

/*
==============================================================================

						SCRIPT COMMANDS

==============================================================================
*/

/*
===============
Cmd_StuffCmds_f

Adds command line parameters as script statements
Commands lead with a +, and continue until a - or another +
quake +prog jctest.qp +cmd amlev1
quake -nosound +cmd amlev1
===============
*/
void Cmd_StuffCmds_f()
{
	int i, j;
	int s;
	char *text, *build, c;

	if(Cmd_Argc() != 1)
	{
		Con_Printf("stuffcmds : execute command line parameters\n");
		return;
	}

	// build the combined string to parse from
	s = 0;
	for(i = 1; i < com_argc; i++)
	{
		if(!com_argv[i])
			continue; // NEXTSTEP nulls out -NXHost
		s += Q_strlen(com_argv[i]) + 1;
	}
	if(!s)
		return;

	text = Z_Malloc(s + 1);
	text[0] = 0;
	for(i = 1; i < com_argc; i++)
	{
		if(!com_argv[i])
			continue; // NEXTSTEP nulls out -NXHost
		Q_strcat(text, com_argv[i]);
		if(i != com_argc - 1)
			Q_strcat(text, " ");
	}

	// pull out the commands
	build = Z_Malloc(s + 1);
	build[0] = 0;

	for(i = 0; i < s - 1; i++)
	{
		if(text[i] == '+')
		{
			i++;

			for(j = i; (text[j] != '+') && (text[j] != '-') && (text[j] != 0); j++)
				;

			c = text[j];
			text[j] = 0;

			Q_strcat(build, text + i);
			Q_strcat(build, "\n");
			text[j] = c;
			i = j - 1;
		}
	}

	if(build[0])
		Cbuf_InsertText(build);

	Z_Free(text);
	Z_Free(build);
}

/*
===============
Cmd_Exec_f
===============
*/
void Cmd_Exec_f()
{
	char *f;
	int mark;

	if(Cmd_Argc() != 2)
	{
		Con_Printf("exec <filename> : execute a script file\n");
		return;
	}

	// FIXME: is this safe freeing the hunk here???
	mark = Hunk_LowMark();
	f = (char *)COM_LoadHunkFile(Cmd_Argv(1));
	if(!f)
	{
		Con_Printf("couldn't exec %s\n", Cmd_Argv(1));
		return;
	}
	Con_Printf("execing %s\n", Cmd_Argv(1));

	Cbuf_InsertText(f);
	Hunk_FreeToLowMark(mark);
}

/*
===============
Cmd_Echo_f

Just prints the rest of the line to the console
===============
*/
void Cmd_Echo_f()
{
	int i;

	for(i = 1; i < Cmd_Argc(); i++)
		Con_Printf("%s ", Cmd_Argv(i));
	Con_Printf("\n");
}

/*
===============
Cmd_Alias_f

Creates a new command that executes a command string (possibly ; seperated)
===============
*/

char *CopyString(char *in)
{
	char *out;

	out = Z_Malloc(strlen(in) + 1);
	strcpy(out, in);
	return out;
}

void Cmd_Alias_f()
{
	cmdalias_t *a;
	char cmd[1024];
	int i, c;
	char *s;

	if(Cmd_Argc() == 1)
	{
		Con_Printf("Current alias commands:\n");
		for(a = cmd_alias; a; a = a->next)
			Con_Printf("%s : %s\n", a->name, a->value);
		return;
	}

	s = Cmd_Argv(1);
	if(strlen(s) >= MAX_ALIAS_NAME)
	{
		Con_Printf("Alias name is too long\n");
		return;
	}

	// if the alias allready exists, reuse it
	for(a = cmd_alias; a; a = a->next)
	{
		if(!strcmp(s, a->name))
		{
			Z_Free(a->value);
			break;
		}
	}

	if(!a)
	{
		a = Z_Malloc(sizeof(cmdalias_t));
		a->next = cmd_alias;
		cmd_alias = a;
	}
	strcpy(a->name, s);

	// copy the rest of the command line
	cmd[0] = 0; // start out with a null string
	c = Cmd_Argc();
	for(i = 2; i < c; i++)
	{
		strcat(cmd, Cmd_Argv(i));
		if(i != c)
			strcat(cmd, " ");
	}
	strcat(cmd, "\n");

	a->value = CopyString(cmd);
}

/*
============
Cmd_Init
============
*/
void Cmd_Init()
{
	//
	// register our commands
	//
	Cmd_AddCommand("stuffcmds", Cmd_StuffCmds_f);
	Cmd_AddCommand("exec", Cmd_Exec_f);
	Cmd_AddCommand("echo", Cmd_Echo_f);
	Cmd_AddCommand("alias", Cmd_Alias_f);
	Cmd_AddCommand("cmd", Cmd_ForwardToServer);
	Cmd_AddCommand("wait", Cmd_Wait_f);
}

/*
============
Cmd_Args

Returns a single string containing argv(1) to argv(argc()-1)
============
*/
char *Cmd_Args()
{
	if(!cmd_args)
		return "";
	return cmd_args;
}

/*
============
Cmd_TokenizeString

Parses the given string into command line tokens.
============
*/
void Cmd_TokenizeString(char *text)
{
	int i;

	// clear the args from the last string
	for(i = 0; i < cmd_argc; i++)
		Z_Free(cmd_argv[i]);

	cmd_argc = 0;
	cmd_args = NULL;

	while(1)
	{
		// skip whitespace up to a /n
		while(*text && *text <= ' ' && *text != '\n')
		{
			text++;
		}

		if(*text == '\n')
		{ // a newline seperates commands in the buffer
			text++;
			break;
		}

		if(!*text)
			return;

		if(cmd_argc == 1)
			cmd_args = text;

		text = COM_Parse(text);
		if(!text)
			return;

		if(cmd_argc < MAX_ARGS)
		{
			cmd_argv[cmd_argc] = Z_Malloc(Q_strlen(com_token) + 1);
			Q_strcpy(cmd_argv[cmd_argc], com_token);
			cmd_argc++;
		}
	}
}

/*
============
Cmd_CompleteCommand
============
*/
char *Cmd_CompleteCommand(char *partial)
{
	cmd_function_t *cmd;
	int len;

	len = Q_strlen(partial);

	if(!len)
		return NULL;

	// check functions
	for(cmd = cmd_functions; cmd; cmd = cmd->next)
		if(!Q_strncmp(partial, cmd->name, len))
			return cmd->name;

	return NULL;
}

/*
===================
Cmd_ForwardToServer

Sends the entire command line over to the server
===================
*/
void Cmd_ForwardToServer()
{
	if(cls.state != ca_connected)
	{
		Con_Printf("Can't \"%s\", not connected\n", Cmd_Argv(0));
		return;
	}

	if(cls.demoplayback)
		return; // not really connected

	MSG_WriteByte(&cls.message, clc_stringcmd);
	if(Q_strcasecmp(Cmd_Argv(0), "cmd") != 0)
	{
		SZ_Print(&cls.message, Cmd_Argv(0));
		SZ_Print(&cls.message, " ");
	}
	if(Cmd_Argc() > 1)
		SZ_Print(&cls.message, Cmd_Args());
	else
		SZ_Print(&cls.message, "\n");
}

/*
================
Cmd_CheckParm

Returns the position (1 to argc-1) in the command's argument list
where the given parameter apears, or 0 if not present
================
*/

int Cmd_CheckParm(char *parm)
{
	int i;

	if(!parm)
		Sys_Error("Cmd_CheckParm: NULL");

	for(i = 1; i < Cmd_Argc(); i++)
		if(!Q_strcasecmp(parm, Cmd_Argv(i)))
			return i;

	return 0;
}
