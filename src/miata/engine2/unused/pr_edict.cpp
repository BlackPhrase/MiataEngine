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
// sv_edict.c -- entity dictionary

#include "quakedef.hpp"

char *pr_strings;
globalvars_t *pr_global_struct;

cvar_t nomonsters = { "nomonsters", "0" };
cvar_t gamecfg = { "gamecfg", "0" };
cvar_t savedgamecfg = { "savedgamecfg", "0", true };

/*
=================
ED_ClearEdict

Sets everything to NULL
=================
*/
void ED_ClearEdict(edict_t *e)
{
	memset(&e->v, 0, sizeof(e->v));
	e->free = false;
}

//===========================================================================

/*
=============
ED_Print

For debugging
=============
*/
void ED_Print(edict_t *ed)
{
	int l;
	ddef_t *d;
	int *v;
	int i, j;
	char *name;
	int type;

	if(ed->free)
	{
		Con_Printf("FREE\n");
		return;
	}

	Con_Printf("\nEDICT %i:\n", NUM_FOR_EDICT(ed));
	for(i = 1; i < progs->numfielddefs; i++)
	{
		d = &pr_fielddefs[i];
		name = pr_strings + d->s_name;
		if(name[strlen(name) - 2] == '_')
			continue; // skip _x, _y, _z vars

		v = (int *)((char *)&ed->v + d->ofs * 4);

		// if the value is still all 0, skip the field
		type = d->type & ~DEF_SAVEGLOBAL;

		for(j = 0; j < type_size[type]; j++)
			if(v[j])
				break;
		if(j == type_size[type])
			continue;

		Con_Printf("%s", name);
		l = strlen(name);
		while(l++ < 15)
			Con_Printf(" ");

		Con_Printf("%s\n", PR_ValueString(d->type, (eval_t *)v));
	}
}

/*
=============
ED_Write

For savegames
=============
*/
void ED_Write(FILE *f, edict_t *ed)
{
	ddef_t *d;
	int *v;
	int i, j;
	char *name;
	int type;

	fprintf(f, "{\n");

	if(ed->free)
	{
		fprintf(f, "}\n");
		return;
	}

	for(i = 1; i < progs->numfielddefs; i++)
	{
		d = &pr_fielddefs[i];
		name = pr_strings + d->s_name;
		if(name[strlen(name) - 2] == '_')
			continue; // skip _x, _y, _z vars

		v = (int *)((char *)&ed->v + d->ofs * 4);

		// if the value is still all 0, skip the field
		type = d->type & ~DEF_SAVEGLOBAL;
		for(j = 0; j < type_size[type]; j++)
			if(v[j])
				break;
		if(j == type_size[type])
			continue;

		fprintf(f, "\"%s\" ", name);
		fprintf(f, "\"%s\"\n", PR_UglyValueString(d->type, (eval_t *)v));
	}

	fprintf(f, "}\n");
}

void ED_PrintNum(int ent)
{
	ED_Print(EDICT_NUM(ent));
}

/*
=============
ED_PrintEdicts

For debugging, prints all the entities in the current server
=============
*/
void ED_PrintEdicts()
{
	int i;

	Con_Printf("%i entities\n", sv.num_edicts);
	for(i = 0; i < sv.num_edicts; i++)
		ED_PrintNum(i);
};

/*
=============
ED_PrintEdict_f

For debugging, prints a single edicy
=============
*/
void ED_PrintEdict_f()
{
	int i;

	i = Q_atoi(Cmd_Argv(1));
	if(i >= sv.num_edicts)
	{
		Con_Printf("Bad edict number\n");
		return;
	}
	ED_PrintNum(i);
}

/*
=============
ED_Count

For debugging
=============
*/
void ED_Count()
{
	int i;
	edict_t *ent;
	int active, models, solid, step;

	active = models = solid = step = 0;
	for(i = 0; i < sv.num_edicts; i++)
	{
		ent = EDICT_NUM(i);
		if(ent->free)
			continue;
		active++;
		if(ent->v.solid)
			solid++;
		if(ent->v.model)
			models++;
		if(ent->v.movetype == MOVETYPE_STEP)
			step++;
	}

	Con_Printf("num_edicts:%3i\n", sv.num_edicts);
	Con_Printf("active    :%3i\n", active);
	Con_Printf("view      :%3i\n", models);
	Con_Printf("touch     :%3i\n", solid);
	Con_Printf("step      :%3i\n", step);
}

/*
==============================================================================

					ARCHIVING GLOBALS

FIXME: need to tag constants, doesn't really work
==============================================================================
*/

/*
=============
ED_ParseGlobals
=============
*/
void ED_ParseGlobals(char *data)
{
	char keyname[64];
	ddef_t *key;

	while(1)
	{
		// parse key
		data = COM_Parse(data);
		if(com_token[0] == '}')
			break;
		if(!data)
			Sys_Error("ED_ParseEntity: EOF without closing brace");

		strcpy(keyname, com_token);

		// parse value
		data = COM_Parse(data);
		if(!data)
			Sys_Error("ED_ParseEntity: EOF without closing brace");

		if(com_token[0] == '}')
			Sys_Error("ED_ParseEntity: closing brace without data");

		key = ED_FindGlobal(keyname);
		if(!key)
		{
			Con_Printf("'%s' is not a global\n", keyname);
			continue;
		}

		if(!ED_ParseEpair((void *)pr_globals, key, com_token))
			Host_Error("ED_ParseGlobals: parse error");
	}
}

//============================================================================

/*
=============
ED_NewString
=============
*/
char *ED_NewString(char *string)
{
	char *pnew, *new_p;
	int i, l;

	l = strlen(string) + 1;
	pnew = Hunk_Alloc(l);
	new_p = pnew;

	for(i = 0; i < l; i++)
	{
		if(string[i] == '\\' && i < l - 1)
		{
			i++;
			if(string[i] == 'n')
				*new_p++ = '\n';
			else
				*new_p++ = '\\';
		}
		else
			*new_p++ = string[i];
	}

	return pnew;
}

/*
====================
ED_ParseEdict

Parses an edict out of the given string, returning the new position
ed should be a properly initialized empty edict.
Used for initial level load and for savegames.
====================
*/
char *ED_ParseEdict(char *data, edict_t *ent)
{
	ddef_t *key;
	qboolean anglehack;
	qboolean init;
	char keyname[256];
	int n;

	init = false;

	// clear it
	if(ent != sv.edicts) // hack
		memset(&ent->v, 0, sizeof(ent->v));

	// go through all the dictionary pairs
	while(1)
	{
		// parse key
		data = COM_Parse(data);
		if(com_token[0] == '}')
			break;
		if(!data)
			Sys_Error("ED_ParseEntity: EOF without closing brace");

		// anglehack is to allow QuakeEd to write single scalar angles
		// and allow them to be turned into vectors. (FIXME...)
		if(!strcmp(com_token, "angle"))
		{
			strcpy(com_token, "angles");
			anglehack = true;
		}
		else
			anglehack = false;

		// FIXME: change light to _light to get rid of this hack
		if(!strcmp(com_token, "light"))
			strcpy(com_token, "light_lev"); // hack for single light def

		strcpy(keyname, com_token);

		// another hack to fix heynames with trailing spaces
		n = strlen(keyname);
		while(n && keyname[n - 1] == ' ')
		{
			keyname[n - 1] = 0;
			n--;
		}

		// parse value
		data = COM_Parse(data);
		if(!data)
			Sys_Error("ED_ParseEntity: EOF without closing brace");

		if(com_token[0] == '}')
			Sys_Error("ED_ParseEntity: closing brace without data");

		init = true;

		// keynames with a leading underscore are used for utility comments,
		// and are immediately discarded by quake
		if(keyname[0] == '_')
			continue;

		key = ED_FindField(keyname);
		if(!key)
		{
			Con_Printf("'%s' is not a field\n", keyname);
			continue;
		}

		if(anglehack)
		{
			char temp[32];
			strcpy(temp, com_token);
			sprintf(com_token, "0 %s 0", temp);
		}

		if(!ED_ParseEpair((void *)&ent->v, key, com_token))
			Host_Error("ED_ParseEdict: parse error");
	}

	if(!init)
		ent->free = true;

	return data;
}

/*
================
ED_LoadFromFile

The entities are directly placed in the array, rather than allocated with
ED_Alloc, because otherwise an error loading the map would have entity
number references out of order.

Creates a server's entity / program execution context by
parsing textual entity definitions out of an ent file.

Used for both fresh maps and savegame loads.  A fresh map would also need
to call ED_CallSpawnFunctions () to let the objects initialize themselves.
================
*/
void ED_LoadFromFile(char *data)
{
	edict_t *ent;
	int inhibit;
	dfunction_t *func;

	ent = NULL;
	inhibit = 0;
	pr_global_struct->time = sv.time;

	// parse ents
	while(1)
	{
		// parse the opening brace
		data = COM_Parse(data);
		if(!data)
			break;
		if(com_token[0] != '{')
			Sys_Error("ED_LoadFromFile: found %s when expecting {", com_token);

		if(!ent)
			ent = EDICT_NUM(0);
		else
			ent = ED_Alloc();
		data = ED_ParseEdict(data, ent);

		// remove things from different skill levels or deathmatch
		if(deathmatch.value)
		{
			if(((int)ent->v.spawnflags & SPAWNFLAG_NOT_DEATHMATCH))
			{
				ED_Free(ent);
				inhibit++;
				continue;
			}
		}
		else if((current_skill == 0 && ((int)ent->v.spawnflags & SPAWNFLAG_NOT_EASY)) || (current_skill == 1 && ((int)ent->v.spawnflags & SPAWNFLAG_NOT_MEDIUM)) || (current_skill >= 2 && ((int)ent->v.spawnflags & SPAWNFLAG_NOT_HARD)))
		{
			ED_Free(ent);
			inhibit++;
			continue;
		}

		//
		// immediately call spawn function
		//
		if(!ent->v.classname)
		{
			Con_Printf("No classname for:\n");
			ED_Print(ent);
			ED_Free(ent);
			continue;
		}

		// look for the spawn function
		func = ED_FindFunction(pr_strings + ent->v.classname);

		if(!func)
		{
			Con_Printf("No spawn function for:\n");
			ED_Print(ent);
			ED_Free(ent);
			continue;
		}

		pr_global_struct->self = EDICT_TO_PROG(ent);
		PR_ExecuteProgram(func - pr_functions);
	}

	Con_DPrintf("%i entities inhibited\n", inhibit);
}

/*
===============
PR_Init
===============
*/
void PR_Init()
{
	Cmd_AddCommand("edict", ED_PrintEdict_f);
	Cmd_AddCommand("edicts", ED_PrintEdicts);
	Cmd_AddCommand("edictcount", ED_Count);
	Cmd_AddCommand("profile", PR_Profile_f);
	
	Cvar_RegisterVariable(&nomonsters);
	Cvar_RegisterVariable(&gamecfg);
	Cvar_RegisterVariable(&savedgamecfg);
}

edict_t *EDICT_NUM(int n)
{
	if(n < 0 || n >= sv.max_edicts)
		Sys_Error("EDICT_NUM: bad number %i", n);
	return (edict_t *)((byte *)sv.edicts + (n)*pr_edict_size);
}

int NUM_FOR_EDICT(edict_t *e)
{
	int b;

	b = (byte *)e - (byte *)sv.edicts;
	b = b / pr_edict_size;

	if(b < 0 || b >= sv.num_edicts)
		Sys_Error("NUM_FOR_EDICT: bad pointer");
	return b;
}
