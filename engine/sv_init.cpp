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

#include "quakedef.hpp"

server_t		sv;
server_static_t	svs;

char	localmodels[MAX_MODELS][5];			// inline model names for precache

/*
================
SV_ModelIndex

================
*/
int SV_ModelIndex (char *name)
{
	int		i;
	
	if (!name || !name[0])
		return 0;

	for (i=0 ; i<MAX_MODELS && sv.model_precache[i] ; i++)
		if (!strcmp(sv.model_precache[i], name))
			return i;
	if (i==MAX_MODELS || !sv.model_precache[i])
		Sys_Error ("SV_ModelIndex: model %s not precached", name);
	return i;
}

/*
================
SV_FlushSignon

Moves to the next signon buffer if needed
================
*/
void SV_FlushSignon ()
{
	if (sv.signon.cursize < sv.signon.maxsize - 512)
		return;

	if (sv.num_signon_buffers == MAX_SIGNON_BUFFERS-1)
		SV_Error ("sv.num_signon_buffers == MAX_SIGNON_BUFFERS-1");

	sv.signon_buffer_size[sv.num_signon_buffers-1] = sv.signon.cursize;
	sv.signon.data = sv.signon_buffers[sv.num_signon_buffers];
	sv.num_signon_buffers++;
	sv.signon.cursize = 0;
}

/*
================
SV_CreateBaseline

Entity baselines are used to compress the update messages
to the clients -- only the fields that differ from the
baseline will be transmitted
================
*/
void SV_CreateBaseline (void)
{
	int			i;
	edict_t			*svent;
	int				entnum;	
		
	for (entnum = 0; entnum < sv.num_edicts ; entnum++)
	{
	// get the current server version
		svent = EDICT_NUM(entnum);
		
		if (svent->free)
			continue;
		
		// create baselines for all player slots,
		// and any other edict that has a visible model
		if (entnum > svs.maxclients && !svent->v.modelindex) // MAX_CLIENTS
			continue;

	//
	// create entity baseline
	//
		VectorCopy (svent->v.origin, svent->baseline.origin);
		VectorCopy (svent->v.angles, svent->baseline.angles);
		svent->baseline.frame = svent->v.frame;
		svent->baseline.skin = svent->v.skin; // svent->baseline.skinnum
		if (entnum > 0 && entnum <= svs.maxclients) // MAX_CLIENTS
		{
			svent->baseline.colormap = entnum;
			svent->baseline.modelindex = SV_ModelIndex("progs/player.mdl");
		}
		else
		{
			svent->baseline.colormap = 0;
			svent->baseline.modelindex =
				SV_ModelIndex(pr_strings + svent->v.model);
		}
		
		//
		// flush the signon message out to a seperate buffer if
		// nearly full
		//
		SV_FlushSignon ();
		
		//
		// add to the message
		//
		MSG_WriteByte (&sv.signon,svc_spawnbaseline);		
		MSG_WriteShort (&sv.signon,entnum);

		MSG_WriteByte (&sv.signon, svent->baseline.modelindex);
		MSG_WriteByte (&sv.signon, svent->baseline.frame);
		MSG_WriteByte (&sv.signon, svent->baseline.colormap);
		MSG_WriteByte (&sv.signon, svent->baseline.skin); // skinnum
		for (i=0 ; i<3 ; i++)
		{
			MSG_WriteCoord(&sv.signon, svent->baseline.origin[i]);
			MSG_WriteAngle(&sv.signon, svent->baseline.angles[i]);
		}
	}
}

/*
================
SV_SaveSpawnparms

Grabs the current state of each client for saving across the
transition to another level
================
*/
void SV_SaveSpawnparms (void)
{
	int		i, j;

	if (!sv.state)
		return;		// no progs loaded yet
	
	// serverflags is the only game related thing maintained
	svs.serverflags = pr_global_struct->serverflags;

	for (i=0, host_client = svs.clients ; i<svs.maxclients ; i++, host_client++)
	{
		if (!host_client->active)
			continue;

	// call the progs to get default spawn parms for the new client
		pr_global_struct->self = EDICT_TO_PROG(host_client->edict);
		PR_ExecuteProgram (pr_global_struct->SetChangeParms);
		for (j=0 ; j<NUM_SPAWN_PARMS ; j++)
			host_client->spawn_parms[j] = (&pr_global_struct->parm1)[j];
	}
}

/*
================
SV_CalcPHS

Expands the PVS and calculates the PHS
(Potentially Hearable Set)
================
*/
void SV_CalcPHS (void)
{
	int		rowbytes, rowwords;
	int		i, j, k, l, index, num;
	int		bitbyte;
	unsigned	*dest, *src;
	byte	*scan;
	int		count, vcount;

	Con_Printf ("Building PHS...\n");

	num = sv.worldmodel->numleafs;
	rowwords = (num+31)>>5;
	rowbytes = rowwords*4;

	sv.pvs = Hunk_Alloc (rowbytes*num);
	scan = sv.pvs;
	vcount = 0;
	for (i=0 ; i<num ; i++, scan+=rowbytes)
	{
		memcpy (scan, Mod_LeafPVS(sv.worldmodel->leafs+i, sv.worldmodel),
			rowbytes);
		if (i == 0)
			continue;
		for (j=0 ; j<num ; j++)
		{
			if ( scan[j>>3] & (1<<(j&7)) )
			{
				vcount++;
			}
		}
	}


	sv.phs = Hunk_Alloc (rowbytes*num);
	count = 0;
	scan = sv.pvs;
	dest = (unsigned *)sv.phs;
	for (i=0 ; i<num ; i++, dest += rowwords, scan += rowbytes)
	{
		memcpy (dest, scan, rowbytes);
		for (j=0 ; j<rowbytes ; j++)
		{
			bitbyte = scan[j];
			if (!bitbyte)
				continue;
			for (k=0 ; k<8 ; k++)
			{
				if (! (bitbyte & (1<<k)) )
					continue;
				// or this pvs row into the phs
				// +1 because pvs is 1 based
				index = ((j<<3)+k+1);
				if (index >= num)
					continue;
				src = (unsigned *)sv.pvs + index*rowwords;
				for (l=0 ; l<rowwords ; l++)
					dest[l] |= src[l];
			}
		}

		if (i == 0)
			continue;
		for (j=0 ; j<num ; j++)
			if ( ((byte *)dest)[j>>3] & (1<<(j&7)) )
				count++;
	}

	Con_Printf ("Average leafs visible / hearable / total: %i / %i / %i\n"
		, vcount/num, count/num, num);
}

unsigned SV_CheckModel(char *mdl)
{
	byte	stackbuf[1024];		// avoid dirtying the cache heap
	byte *buf;
	unsigned short crc;
//	int len;

	buf = (byte *)COM_LoadStackFile (mdl, stackbuf, sizeof(stackbuf));
	crc = CRC_Block(buf, com_filesize);
//	for (len = com_filesize; len; len--, buf++)
//		CRC_ProcessByte(&crc, *buf);

	return crc;
}

/*
================
SV_SpawnServer

This is called at the start of each level
================
*/
extern float		scr_centertime_off;

#ifdef QUAKE2
void SV_SpawnServer (char *server, char *startspot)
#else
void SV_SpawnServer (char *server)
#endif
{
	edict_t		*ent;
	int			i;

	// let's not have any servers with no name
	if (hostname.string[0] == 0)
		Cvar_Set ("hostname", "UNNAMED");
	scr_centertime_off = 0;

	Con_DPrintf ("SpawnServer: %s\n",server);
	svs.changelevel_issued = false;		// now safe to issue another

//
// tell all connected clients that we are going to a new level
//
	if (sv.active)
	{
		SV_SendReconnect ();
	}

//
// make cvars consistant
//
	if (coop.value)
		Cvar_SetValue ("deathmatch", 0);
	current_skill = (int)(skill.value + 0.5);
	if (current_skill < 0)
		current_skill = 0;
	if (current_skill > 3)
		current_skill = 3;

	Cvar_SetValue ("skill", (float)current_skill);
	
//
// set up the new server
//
	Host_ClearMemory ();

	memset (&sv, 0, sizeof(sv));

	strcpy (sv.name, server);
#ifdef QUAKE2
	if (startspot)
		strcpy(sv.startspot, startspot);
#endif

// load progs to get entity field count
	PR_LoadProgs ();

// allocate server memory
	sv.max_edicts = MAX_EDICTS;
	
	sv.edicts = Hunk_AllocName (sv.max_edicts*pr_edict_size, "edicts");

	sv.datagram.maxsize = sizeof(sv.datagram_buf);
	sv.datagram.cursize = 0;
	sv.datagram.data = sv.datagram_buf;
	
	sv.reliable_datagram.maxsize = sizeof(sv.reliable_datagram_buf);
	sv.reliable_datagram.cursize = 0;
	sv.reliable_datagram.data = sv.reliable_datagram_buf;
	
	sv.signon.maxsize = sizeof(sv.signon_buf);
	sv.signon.cursize = 0;
	sv.signon.data = sv.signon_buf;
	
// leave slots at start for clients only
	sv.num_edicts = svs.maxclients+1;
	for (i=0 ; i<svs.maxclients ; i++)
	{
		ent = EDICT_NUM(i+1);
		svs.clients[i].edict = ent;
	}
	
	sv.state = ss_loading;
	sv.paused = false;

	sv.time = 1.0;
	
	strcpy (sv.name, server);
	sprintf (sv.modelname,"maps/%s.bsp", server);
	sv.worldmodel = Mod_ForName (sv.modelname, false);
	if (!sv.worldmodel)
	{
		Con_Printf ("Couldn't spawn server %s\n", sv.modelname);
		sv.active = false;
		return;
	}
	sv.models[1] = sv.worldmodel;
	
//
// clear world interaction links
//
	SV_ClearWorld ();
	
	sv.sound_precache[0] = pr_strings;

	sv.model_precache[0] = pr_strings;
	sv.model_precache[1] = sv.modelname;
	for (i=1 ; i<sv.worldmodel->numsubmodels ; i++)
	{
		sv.model_precache[1+i] = localmodels[i];
		sv.models[i+1] = Mod_ForName (localmodels[i], false);
	}

//
// load the rest of the entities
//	
	ent = EDICT_NUM(0);
	memset (&ent->v, 0, progs->entityfields * 4);
	ent->free = false;
	ent->v.model = sv.worldmodel->name - pr_strings;
	ent->v.modelindex = 1;		// world model
	ent->v.solid = SOLID_BSP;
	ent->v.movetype = MOVETYPE_PUSH;

	if (coop.value)
		pr_global_struct->coop = coop.value;
	else
		pr_global_struct->deathmatch = deathmatch.value;

	pr_global_struct->mapname = sv.name - pr_strings;
#ifdef QUAKE2
	pr_global_struct->startspot = sv.startspot - pr_strings;
#endif

// serverflags are for cross level information (sigils)
	pr_global_struct->serverflags = svs.serverflags;
	
	ED_LoadFromFile (sv.worldmodel->entities);

	sv.active = true;

// all setup is completed, any further precache statements are errors
	sv.state = ss_active;
	
// run two frames to allow everything to settle
	host_frametime = 0.1;
	SV_Physics ();
	SV_Physics ();

// create a baseline for more efficient communications
	SV_CreateBaseline ();

// send serverinfo to all connected clients
	for (i=0,host_client = svs.clients ; i<svs.maxclients ; i++, host_client++)
		if (host_client->active)
			SV_SendServerinfo (host_client);
	
	Con_DPrintf ("Server spawned.\n");
}