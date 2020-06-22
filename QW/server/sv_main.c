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

#include "qwsvdef.h"

quakeparms_t host_parms;

qboolean	host_initialized;		// true if into command execution (compatability)

double		host_frametime;
double		realtime;				// without any filtering or bounding

int			host_hunklevel;

netadr_t	master_adr[MAX_MASTERS];	// address of group servers

client_t	*host_client;			// current client

cvar_t	sv_mintic = {"sv_mintic","0.03"};	// bound the size of the
cvar_t	sv_maxtic = {"sv_maxtic","0.1"};	// physics time tic 

cvar_t	developer = {"developer","0"};		// show extra messages
cvar_t	allow_download = {"allow_download", "1"};
cvar_t	allow_download_skins = {"allow_download_skins", "1"};
cvar_t	allow_download_models = {"allow_download_models", "1"};
cvar_t	allow_download_sounds = {"allow_download_sounds", "1"};
cvar_t	allow_download_maps = {"allow_download_maps", "1"};

cvar_t sv_phs = {"sv_phs", "1"};

cvar_t pausable	= {"pausable", "1"};


//
// game rules mirrored in svs.info
//
cvar_t	fraglimit = {"fraglimit","0",false,true};
cvar_t	timelimit = {"timelimit","0",false,true};
cvar_t	teamplay = {"teamplay","0",false,true};
cvar_t	samelevel = {"samelevel","0", false, true};
cvar_t	maxclients = {"maxclients","8", false, true};
cvar_t	maxspectators = {"maxspectators","8", false, true};
cvar_t	deathmatch = {"deathmatch","1", false, true};			// 0, 1, or 2
cvar_t	spawn = {"spawn","0", false, true};
cvar_t	watervis = {"watervis", "0", false, true};

cvar_t	hostname = {"hostname","unnamed", false, true};

FILE	*sv_logfile;
FILE	*sv_fraglogfile;

void SV_AcceptClient (netadr_t adr, int userid, char *userinfo);
void Master_Shutdown (void);

//============================================================================

qboolean ServerPaused(void)
{
	return sv.paused;
}

/*
================
SV_Shutdown

Quake calls this before calling Sys_Quit or Sys_Error
================
*/
void SV_Shutdown (void)
{
	Master_Shutdown ();
	if (sv_logfile)
	{
		fclose (sv_logfile);
		sv_logfile = NULL;
	}
	if (sv_fraglogfile)
	{
		fclose (sv_fraglogfile);
		sv_logfile = NULL;
	}
	NET_Shutdown ();
}

/*
================
SV_Error

Sends a datagram to all the clients informing them of the server crash,
then exits
================
*/
void SV_Error (char *error, ...)
{
	va_list		argptr;
	static	char		string[1024];
	static	qboolean inerror = false;

	if (inerror)
		Sys_Error ("SV_Error: recursively entered (%s)", string);

	inerror = true;

	va_start (argptr,error);
	vsprintf (string,error,argptr);
	va_end (argptr);

	Con_Printf ("SV_Error: %s\n",string);

	SV_FinalMessage (va("server crashed: %s\n", string));
		
	SV_Shutdown ();

	Sys_Error ("SV_Error: %s\n",string);
}

/*
==================
SV_FinalMessage

Used by SV_Error and SV_Quit_f to send a final message to all connected
clients before the server goes down.  The messages are sent immediately,
not just stuck on the outgoing message list, because the server is going
to totally exit after returning from this function.
==================
*/
void SV_FinalMessage (char *message)
{
	int			i;
	client_t	*cl;
	
	SZ_Clear (&net_message);
	MSG_WriteByte (&net_message, svc_print);
	MSG_WriteByte (&net_message, PRINT_HIGH);
	MSG_WriteString (&net_message, message);
	MSG_WriteByte (&net_message, svc_disconnect);

	for (i=0, cl = svs.clients ; i<MAX_CLIENTS ; i++, cl++)
		if (cl->state >= cs_spawned)
			Netchan_Transmit (&cl->netchan, net_message.cursize
			, net_message.data);
}



/*
=====================
SV_DropClient

Called when the player is totally leaving the server, either willingly
or unwillingly.  This is NOT called if the entire server is quiting
or crashing.
=====================
*/
void SV_DropClient (client_t *drop)
{
	// add the disconnect
	MSG_WriteByte (&drop->netchan.message, svc_disconnect);

	if (drop->state == cs_spawned)
		if (!drop->spectator)
		{
			// call the prog function for removing a client
			// this will set the body to a dead frame, among other things
			pr_global_struct->self = EDICT_TO_PROG(drop->edict);
			PR_ExecuteProgram (pr_global_struct->ClientDisconnect);
		}
		else if (SpectatorDisconnect)
		{
			// call the prog function for removing a client
			// this will set the body to a dead frame, among other things
			pr_global_struct->self = EDICT_TO_PROG(drop->edict);
			PR_ExecuteProgram (SpectatorDisconnect);
		}

	if (drop->spectator)
		Con_Printf ("Spectator %s removed\n",drop->name);
	else
		Con_Printf ("Client %s removed\n",drop->name);

	if (drop->download)
	{
		fclose (drop->download);
		drop->download = NULL;
	}
	if (drop->upload)
	{
		fclose (drop->upload);
		drop->upload = NULL;
	}
	*drop->uploadfn = 0;

	drop->state = cs_zombie;		// become free in a few seconds
	drop->connection_started = realtime;	// for zombie timeout

	drop->old_frags = 0;
	drop->edict->v.frags = 0;
	drop->name[0] = 0;
	memset (drop->userinfo, 0, sizeof(drop->userinfo));

// send notification to all remaining clients
	SV_FullClientUpdate (drop, &sv.reliable_datagram);
}


//====================================================================

/*
===================
SV_CalcPing

===================
*/
int SV_CalcPing (client_t *cl)
{
	float		ping;
	int			i;
	int			count;
	register	client_frame_t *frame;

	ping = 0;
	count = 0;
	for (frame = cl->frames, i=0 ; i<UPDATE_BACKUP ; i++, frame++)
	{
		if (frame->ping_time > 0)
		{
			ping += frame->ping_time;
			count++;
		}
	}
	if (!count)
		return 9999;
	ping /= count;

	return ping*1000;
}

/*
===================
SV_FullClientUpdate

Writes all update values to a sizebuf
===================
*/
void SV_FullClientUpdate (client_t *client, sizebuf_t *buf)
{
	int		i;
	char	info[MAX_INFO_STRING];

	i = client - svs.clients;

//Sys_Printf("SV_FullClientUpdate:  Updated frags for client %d\n", i);

	MSG_WriteByte (buf, svc_updatefrags);
	MSG_WriteByte (buf, i);
	MSG_WriteShort (buf, client->old_frags);
	
	MSG_WriteByte (buf, svc_updateping);
	MSG_WriteByte (buf, i);
	MSG_WriteShort (buf, SV_CalcPing (client));
	
	MSG_WriteByte (buf, svc_updatepl);
	MSG_WriteByte (buf, i);
	MSG_WriteByte (buf, client->lossage);
	
	MSG_WriteByte (buf, svc_updateentertime);
	MSG_WriteByte (buf, i);
	MSG_WriteFloat (buf, realtime - client->connection_started);

	strcpy (info, client->userinfo);
	Info_RemovePrefixedKeys (info, '_');	// server passwords, etc

	MSG_WriteByte (buf, svc_updateuserinfo);
	MSG_WriteByte (buf, i);
	MSG_WriteLong (buf, client->userid);
	MSG_WriteString (buf, info);
}

/*
===================
SV_FullClientUpdateToClient

Writes all update values to a client's reliable stream
===================
*/
void SV_FullClientUpdateToClient (client_t *client, client_t *cl)
{
	ClientReliableCheckBlock(cl, 24 + strlen(client->userinfo));
	if (cl->num_backbuf) {
		SV_FullClientUpdate (client, &cl->backbuf);
		ClientReliable_FinishWrite(cl);
	} else
		SV_FullClientUpdate (client, &cl->netchan.message);
}

/*
===================
SV_CheckLog

===================
*/
#define	LOG_HIGHWATER	4096
#define	LOG_FLUSH		10*60
void SV_CheckLog (void)
{
	sizebuf_t	*sz;

	sz = &svs.log[svs.logsequence&1];

	// bump sequence if allmost full, or ten minutes have passed and
	// there is something still sitting there
	if (sz->cursize > LOG_HIGHWATER
	|| (realtime - svs.logtime > LOG_FLUSH && sz->cursize) )
	{
		// swap buffers and bump sequence
		svs.logtime = realtime;
		svs.logsequence++;
		sz = &svs.log[svs.logsequence&1];
		sz->cursize = 0;
		Con_Printf ("beginning fraglog sequence %i\n", svs.logsequence);
	}

}

//============================================================================

/*
===================
SV_GetConsoleCommands

Add them exactly as if they had been typed at the console
===================
*/
void SV_GetConsoleCommands (void)
{
	char	*cmd;

	while (1)
	{
		cmd = Sys_ConsoleInput ();
		if (!cmd)
			break;
		Cbuf_AddText (cmd);
	}
}

/*
===================
SV_CheckVars

===================
*/
void SV_CheckVars (void)
{
	static char *pw, *spw;
	int			v;

	if (password.string == pw && spectator_password.string == spw)
		return;
	pw = password.string;
	spw = spectator_password.string;

	v = 0;
	if (pw && pw[0] && strcmp(pw, "none"))
		v |= 1;
	if (spw && spw[0] && strcmp(spw, "none"))
		v |= 2;

	Con_Printf ("Updated needpass.\n");
	if (!v)
		Info_SetValueForKey (svs.info, "needpass", "", MAX_SERVERINFO_STRING);
	else
		Info_SetValueForKey (svs.info, "needpass", va("%i",v), MAX_SERVERINFO_STRING);
}

/*
==================
SV_Frame

==================
*/
void SV_Frame (float time)
{
	static double	start, end;
	
	start = Sys_DoubleTime ();
	svs.stats.idle += start - end;
	
// keep the random time dependent
	rand ();

// decide the simulation time
	if (!sv.paused) {
		realtime += time;
		sv.time += time;
	}

// check timeouts
	SV_CheckTimeouts ();

// toggle the log buffer if full
	SV_CheckLog ();

// move autonomous things around if enough time has passed
	if (!sv.paused)
		SV_Physics ();

// get packets
	SV_ReadPackets ();

// check for commands typed to the host
	SV_GetConsoleCommands ();
	
// process console commands
	Cbuf_Execute ();

	SV_CheckVars ();

// send messages back to the clients that had packets read this frame
	SV_SendClientMessages ();

// send a heartbeat to the master if needed
	Master_Heartbeat ();

// collect timing statistics
	end = Sys_DoubleTime ();
	svs.stats.active += end-start;
	if (++svs.stats.count == STATFRAMES)
	{
		svs.stats.latched_active = svs.stats.active;
		svs.stats.latched_idle = svs.stats.idle;
		svs.stats.latched_packets = svs.stats.packets;
		svs.stats.active = 0;
		svs.stats.idle = 0;
		svs.stats.packets = 0;
		svs.stats.count = 0;
	}
}

/*
===============
SV_InitLocal
===============
*/
void SV_InitLocal (void)
{
	int		i;
	extern	cvar_t	sv_maxvelocity;
	extern	cvar_t	sv_gravity;
	extern	cvar_t	sv_aim;
	extern	cvar_t	sv_stopspeed;
	extern	cvar_t	sv_spectatormaxspeed;
	extern	cvar_t	sv_accelerate;
	extern	cvar_t	sv_airaccelerate;
	extern	cvar_t	sv_wateraccelerate;
	extern	cvar_t	sv_friction;
	extern	cvar_t	sv_waterfriction;

	SV_InitOperatorCommands	();
	SV_UserInit ();

	Cvar_RegisterVariable (&sv_mintic);
	Cvar_RegisterVariable (&sv_maxtic);

	Cvar_RegisterVariable (&fraglimit);
	Cvar_RegisterVariable (&timelimit);
	Cvar_RegisterVariable (&teamplay);
	Cvar_RegisterVariable (&samelevel);
	Cvar_RegisterVariable (&maxclients);
	Cvar_RegisterVariable (&maxspectators);
	Cvar_RegisterVariable (&hostname);
	Cvar_RegisterVariable (&deathmatch);
	Cvar_RegisterVariable (&spawn);
	Cvar_RegisterVariable (&watervis);

	Cvar_RegisterVariable (&developer);

	Cvar_RegisterVariable (&timeout);
	Cvar_RegisterVariable (&zombietime);

	Cvar_RegisterVariable (&sv_maxvelocity);
	Cvar_RegisterVariable (&sv_gravity);
	Cvar_RegisterVariable (&sv_stopspeed);
	Cvar_RegisterVariable (&sv_maxspeed);
	Cvar_RegisterVariable (&sv_spectatormaxspeed);
	Cvar_RegisterVariable (&sv_accelerate);
	Cvar_RegisterVariable (&sv_airaccelerate);
	Cvar_RegisterVariable (&sv_wateraccelerate);
	Cvar_RegisterVariable (&sv_friction);
	Cvar_RegisterVariable (&sv_waterfriction);

	Cvar_RegisterVariable (&sv_aim);

	Cvar_RegisterVariable (&filterban);
	
	Cvar_RegisterVariable (&allow_download);
	Cvar_RegisterVariable (&allow_download_skins);
	Cvar_RegisterVariable (&allow_download_models);
	Cvar_RegisterVariable (&allow_download_sounds);
	Cvar_RegisterVariable (&allow_download_maps);

	Cvar_RegisterVariable (&sv_highchars);

	Cvar_RegisterVariable (&sv_phs);

	Cvar_RegisterVariable (&pausable);

	Cmd_AddCommand ("addip", SV_AddIP_f);
	Cmd_AddCommand ("removeip", SV_RemoveIP_f);
	Cmd_AddCommand ("listip", SV_ListIP_f);
	Cmd_AddCommand ("writeip", SV_WriteIP_f);

	for (i=0 ; i<MAX_MODELS ; i++)
		sprintf (localmodels[i], "*%i", i);

	Info_SetValueForStarKey (svs.info, "*version", va("%4.2f", VERSION), MAX_SERVERINFO_STRING);

	// init fraglog stuff
	svs.logsequence = 1;
	svs.logtime = realtime;
	svs.log[0].data = svs.log_buf[0];
	svs.log[0].maxsize = sizeof(svs.log_buf[0]);
	svs.log[0].cursize = 0;
	svs.log[0].allowoverflow = true;
	svs.log[1].data = svs.log_buf[1];
	svs.log[1].maxsize = sizeof(svs.log_buf[1]);
	svs.log[1].cursize = 0;
	svs.log[1].allowoverflow = true;
}


//============================================================================

/*
================
Master_Heartbeat

Send a message to the master every few minutes to
let it know we are alive, and log information
================
*/
#define	HEARTBEAT_SECONDS	300
void Master_Heartbeat (void)
{
	char		string[2048];
	int			active;
	int			i;

	if (realtime - svs.last_heartbeat < HEARTBEAT_SECONDS)
		return;		// not time to send yet

	svs.last_heartbeat = realtime;

	//
	// count active users
	//
	active = 0;
	for (i=0 ; i<MAX_CLIENTS ; i++)
		if (svs.clients[i].state == cs_connected ||
		svs.clients[i].state == cs_spawned )
			active++;

	svs.heartbeat_sequence++;
	sprintf (string, "%c\n%i\n%i\n", S2M_HEARTBEAT,
		svs.heartbeat_sequence, active);


	// send to group master
	for (i=0 ; i<MAX_MASTERS ; i++)
		if (master_adr[i].port)
		{
			Con_Printf ("Sending heartbeat to %s\n", NET_AdrToString (master_adr[i]));
			NET_SendPacket (strlen(string), string, master_adr[i]);
		}
}

/*
=================
Master_Shutdown

Informs all masters that this server is going down
=================
*/
void Master_Shutdown (void)
{
	char		string[2048];
	int			i;

	sprintf (string, "%c\n", S2M_SHUTDOWN);

	// send to group master
	for (i=0 ; i<MAX_MASTERS ; i++)
		if (master_adr[i].port)
		{
			Con_Printf ("Sending heartbeat to %s\n", NET_AdrToString (master_adr[i]));
			NET_SendPacket (strlen(string), string, master_adr[i]);
		}
}

/*
=================
SV_ExtractFromUserinfo

Pull specific info from a newly changed userinfo string
into a more C freindly form.
=================
*/
void SV_ExtractFromUserinfo (client_t *cl)
{
	char	*val, *p, *q;
	int		i;
	client_t	*client;
	int		dupc = 1;
	char	newname[80];


	// name for C code
	val = Info_ValueForKey (cl->userinfo, "name");

	// trim user name
	strncpy(newname, val, sizeof(newname) - 1);
	newname[sizeof(newname) - 1] = 0;

	for (p = newname; (*p == ' ' || *p == '\r' || *p == '\n') && *p; p++)
		;

	if (p != newname && !*p) {
		//white space only
		strcpy(newname, "unnamed");
		p = newname;
	}

	if (p != newname && *p) {
		for (q = newname; *p; *q++ = *p++)
			;
		*q = 0;
	}
	for (p = newname + strlen(newname) - 1; p != newname && (*p == ' ' || *p == '\r' || *p == '\n') ; p--)
		;
	p[1] = 0;

	if (strcmp(val, newname)) {
		Info_SetValueForKey (cl->userinfo, "name", newname, MAX_INFO_STRING);
		val = Info_ValueForKey (cl->userinfo, "name");
	}

	if (!val[0] || !stricmp(val, "console")) {
		Info_SetValueForKey (cl->userinfo, "name", "unnamed", MAX_INFO_STRING);
		val = Info_ValueForKey (cl->userinfo, "name");
	}

	// check to see if another user by the same name exists
	while (1) {
		for (i=0, client = svs.clients ; i<MAX_CLIENTS ; i++, client++) {
			if (client->state != cs_spawned || client == cl)
				continue;
			if (!stricmp(client->name, val))
				break;
		}
		if (i != MAX_CLIENTS) { // dup name
			if (strlen(val) > sizeof(cl->name) - 1)
				val[sizeof(cl->name) - 4] = 0;
			p = val;

			if (val[0] == '(')
				if (val[2] == ')')
					p = val + 3;
				else if (val[3] == ')')
					p = val + 4;

			sprintf(newname, "(%d)%-.40s", dupc++, p);
			Info_SetValueForKey (cl->userinfo, "name", newname, MAX_INFO_STRING);
			val = Info_ValueForKey (cl->userinfo, "name");
		} else
			break;
	}
	
	if (strncmp(val, cl->name, strlen(cl->name))) {
		if (!sv.paused) {
			if (!cl->lastnametime || realtime - cl->lastnametime > 5) {
				cl->lastnamecount = 0;
				cl->lastnametime = realtime;
			} else if (cl->lastnamecount++ > 4) {
				SV_BroadcastPrintf (PRINT_HIGH, "%s was kicked for name spam\n", cl->name);
				SV_ClientPrintf (cl, PRINT_HIGH, "You were kicked from the game for name spamming\n");
				SV_DropClient (cl); 
				return;
			}
		}
				
		if (cl->state >= cs_spawned && !cl->spectator)
			SV_BroadcastPrintf (PRINT_HIGH, "%s changed name to %s\n", cl->name, val);
	}


	strncpy (cl->name, val, sizeof(cl->name)-1);	

	// rate command
	val = Info_ValueForKey (cl->userinfo, "rate");
	if (strlen(val))
	{
		i = atoi(val);
		if (i < 500)
			i = 500;
		if (i > 10000)
			i = 10000;
		cl->netchan.rate = 1.0/i;
	}

	// msg command
	val = Info_ValueForKey (cl->userinfo, "msg");
	if (strlen(val))
	{
		cl->messagelevel = atoi(val);
	}

}


//============================================================================

/*
====================
SV_InitNet
====================
*/
void SV_InitNet (void)
{
	int	port;
	int	p;

	port = PORT_SERVER;
	p = COM_CheckParm ("-port");
	if (p && p < com_argc)
	{
		port = atoi(com_argv[p+1]);
		Con_Printf ("Port: %i\n", port);
	}
	NET_Init (port);

	Netchan_Init ();

	// heartbeats will allways be sent to the id master
	svs.last_heartbeat = -99999;		// send immediately
//	NET_StringToAdr ("192.246.40.70:27000", &idmaster_adr);
}


/*
====================
SV_Init
====================
*/
void SV_Init (quakeparms_t *parms)
{
	COM_InitArgv (parms->argc, parms->argv);
	COM_AddParm ("-game");
	COM_AddParm ("qw");

	if (COM_CheckParm ("-minmemory"))
		parms->memsize = MINIMUM_MEMORY;

	host_parms = *parms;

	if (parms->memsize < MINIMUM_MEMORY)
		SV_Error ("Only %4.1f megs of memory reported, can't execute game", parms->memsize / (float)0x100000);

	Memory_Init (parms->membase, parms->memsize);
	Cbuf_Init ();
	Cmd_Init ();	

	COM_Init ();
	
	PR_Init ();
	Mod_Init ();

	SV_InitNet ();

	SV_InitLocal ();
	Sys_Init ();
	Pmove_Init ();

	Hunk_AllocName (0, "-HOST_HUNKLEVEL-");
	host_hunklevel = Hunk_LowMark ();

	Cbuf_InsertText ("exec server.cfg\n");

	host_initialized = true;
	
	Con_Printf ("Exe: "__TIME__" "__DATE__"\n");
	Con_Printf ("%4.1f megabyte heap\n",parms->memsize/ (1024*1024.0));	

	Con_Printf ("\nServer Version %4.2f (Build %04d)\n\n", VERSION, build_number());

	Con_Printf ("======== QuakeWorld Initialized ========\n");
	
// process command line arguments
	Cmd_StuffCmds_f ();
	Cbuf_Execute ();

// if a map wasn't specified on the command line, spawn start.map
	if (sv.state == ss_dead)
		Cmd_ExecuteString ("map start");
	if (sv.state == ss_dead)
		SV_Error ("Couldn't spawn a server");
}
