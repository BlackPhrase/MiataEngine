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
// cvar.c -- dynamic variable tracking

#include "quakedef.hpp"

/*
============
Cvar_CompleteVariable
============
*/
const char *Cvar_CompleteVariable(const char *partial)
{
	cvar_t *cvar;
	int len;

	len = Q_strlen(partial);

	if(!len)
		return NULL;

	// check functions
	for(cvar = cvar_vars; cvar; cvar = cvar->next)
		if(!Q_strncmp(partial, cvar->name, len))
			return cvar->name;

	return NULL;
}

/*
============
Cvar_WriteVariables

Writes lines containing "set variable value" for all variables
with the archive flag set to true.
============
*/
void Cvar_WriteVariables(FILE *f)
{
	for(cvar_t *var = cvar_vars; var; var = var->next)
		if(var->archive)
			fprintf(f, "%s \"%s\"\n", var->name, var->string);
};