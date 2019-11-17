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
// sv_phys.c

#include "quakedef.h"

cvar_t	sv_friction = {"sv_friction","4",false,true};
cvar_t	sv_stopspeed = {"sv_stopspeed","100"};
cvar_t	sv_gravity = {"sv_gravity","800",false,true};
cvar_t	sv_maxvelocity = {"sv_maxvelocity","2000"};
cvar_t	sv_nostep = {"sv_nostep","0"};

#ifdef QUAKE2
static	vec3_t	vec_origin = {0.0, 0.0, 0.0};
#endif