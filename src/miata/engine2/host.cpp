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
#include "r_local.hpp"

double host_frametime;
double host_time;

double realtime;    // without any filtering or bounding
double oldrealtime; // last frame run

int host_framecount;

int host_hunklevel;

int minimum_memory;

client_t *host_client; // current client

jmp_buf host_abortserver;

byte *host_basepal;
byte *host_colormap;

cvar_t host_framerate = { "host_framerate", "0" }; // set for slow motion
cvar_t host_speeds = { "host_speeds", "0" };       // set for running times

cvar_t sys_ticrate = { "sys_ticrate", "0.05" };
cvar_t serverprofile = { "serverprofile", "0" };

cvar_t fraglimit = { "fraglimit", "0", false, true };
cvar_t timelimit = { "timelimit", "0", false, true };
cvar_t teamplay = { "teamplay", "0", false, true };

cvar_t samelevel = { "samelevel", "0" };
cvar_t noexit = { "noexit", "0", false, true };

#ifdef IDTECH2RED_DEV
cvar_t developer = { "developer", "1" }; // should be 0 for release!
#else
cvar_t developer = { "developer", "0" };
#endif

cvar_t skill = { "skill", "1" };           // 0 - 3
cvar_t deathmatch = { "deathmatch", "0" }; // 0, 1, or 2
cvar_t coop = { "coop", "0" };             // 0 or 1

cvar_t pausable = { "pausable", "1" };

/*
================
Host_EndGame
================
*/
void Host_EndGame(const char *message, ...)
{
	va_list argptr;
	char string[1024]{};

	va_start(argptr, message);
	vsprintf(string, message, argptr);
	va_end(argptr);
	
	mpLogger->DPrintf("Host_EndGame: %s\n", string);

	if(sv.active)
		Host_ShutdownServer(false);

	// dedicated servers exit
	if(cls.state == ca_dedicated)
		Sys_Error("Host_EndGame: %s\n", string);

	if(cls.demonum != -1)
		CL_NextDemo();
	else
		CL_Disconnect();

	longjmp(host_abortserver, 1);
}

/*
================
Host_Error

This shuts down both the client and server
================
*/
void Host_Error(const char *error, ...)
{
	va_list argptr;
	char string[1024]{};
	
	static bool inerror = false;

	if(inerror)
		Sys_Error("Host_Error: recursively entered");
	
	inerror = true;

	// reenable screen updates
	SCR_EndLoadingPlaque();

	va_start(argptr, error);
	vsprintf(string, error, argptr);
	va_end(argptr);
	
	mpLogger->Printf("Host_Error: %s\n", string);

	if(sv.active)
		Host_ShutdownServer(false);

	// dedicated servers exit
	if(cls.state == ca_dedicated)
		Sys_Error("Host_Error: %s\n", string);

	CL_Disconnect();
	cls.demonum = -1;

	inerror = false;

	longjmp(host_abortserver, 1);
}

/*
================
Host_FindMaxClients
================
*/
void Host_FindMaxClients()
{
	svs.maxclients = 1;

	int i = mpCmdLine->CheckParm("-dedicated");
	
	if(i)
	{
		cls.state = ca_dedicated;
		
		if(i != (com_argc - 1))
			svs.maxclients = Q_atoi(com_argv[i + 1]);
		else
			svs.maxclients = 8;
	}
	else
		cls.state = ca_disconnected;

	i = mpCmdLine->CheckParm("-listen");
	
	if(i)
	{
		if(cls.state == ca_dedicated)
			Sys_Error("Only one of -dedicated or -listen can be specified");
		
		if(i != (com_argc - 1))
			svs.maxclients = Q_atoi(com_argv[i + 1]);
		else
			svs.maxclients = 8;
	};
	
	if(svs.maxclients < 1)
		svs.maxclients = 8;
	else if(svs.maxclients > MAX_SCOREBOARD)
		svs.maxclients = MAX_SCOREBOARD;

	svs.maxclientslimit = svs.maxclients;
	
	if(svs.maxclientslimit < 4)
		svs.maxclientslimit = 4;
	
	svs.clients = Hunk_AllocName(svs.maxclientslimit * sizeof(client_t), "clients");

	if(svs.maxclients > 1)
		Cvar_SetValue("deathmatch", 1.0);
	else
		Cvar_SetValue("deathmatch", 0.0);
}

/*
=======================
Host_InitLocal
======================
*/
void Host_InitLocal()
{
	Host_InitCommands();

	mpCvarList->Register(&host_framerate);
	mpCvarList->Register(&host_speeds);

	mpCvarList->Register(&sys_ticrate);
	mpCvarList->Register(&serverprofile);

	mpCvarList->Register(&fraglimit);
	mpCvarList->Register(&timelimit);
	mpCvarList->Register(&teamplay);
	mpCvarList->Register(&samelevel);
	mpCvarList->Register(&noexit);
	mpCvarList->Register(&skill);
	mpCvarList->Register(&developer);
	mpCvarList->Register(&deathmatch);
	mpCvarList->Register(&coop);

	mpCvarList->Register(&pausable);

	Host_FindMaxClients();

	host_time = 1.0; // so a think at time 0 won't get called
}

/*
===============
Host_WriteConfiguration

Writes key bindings and archived cvars to config.cfg
===============
*/
void Host_WriteConfiguration()
{
	// dedicated servers initialize the host but don't parse and set the
	// config.cfg cvars
	if(host_initialized & !isDedicated)
	{
		FILE *f = fopen(va("%s/config.cfg", com_gamedir), "w");
		
		if(!f)
		{
			mpLogger->Printf("Couldn't write config.cfg.\n");
			return;
		};

		Key_WriteBindings(f);
		Cvar_WriteVariables(f);

		fclose(f);
	};
}

/*
==================
Host_ShutdownServer

This only happens at the end of a game, not between levels
==================
*/
void Host_ShutdownServer(bool crash)
{
	int i;
	int count;
	sizebuf_t buf;
	char message[4];
	double start;

	if(!sv.active)
		return;

	sv.active = false;

	// stop all client sounds immediately
	if(cls.state == ca_connected)
		CL_Disconnect();

	// flush any pending messages - like the score!!!
	start = Sys_FloatTime();
	
	do
	{
		count = 0;
		for(i = 0, host_client = svs.clients; i < svs.maxclients; i++, host_client++)
		{
			if(host_client->active && host_client->message.cursize)
			{
				if(NET_CanSendMessage(host_client->netconnection))
				{
					NET_SendMessage(host_client->netconnection, &host_client->message);
					SZ_Clear(&host_client->message);
				}
				else
				{
					NET_GetMessage(host_client->netconnection);
					count++;
				}
			}
		}
		if((Sys_FloatTime() - start) > 3.0)
			break;
	} while(count);

	// make sure all the clients know we're disconnecting
	buf.data = message;
	buf.maxsize = 4;
	buf.cursize = 0;
	MSG_WriteByte(&buf, svc_disconnect);
	count = NET_SendToAll(&buf, 5);
	
	if(count)
		mpLogger->Printf("Host_ShutdownServer: NET_SendToAll failed for %u clients\n", count);

	for(i = 0, host_client = svs.clients; i < svs.maxclients; i++, host_client++)
		if(host_client->active)
			SV_DropClient(host_client, crash, "black magic");

	//
	// clear structures
	//
	memset(&sv, 0, sizeof(sv));
	memset(svs.clients, 0, svs.maxclientslimit * sizeof(client_t));
}

/*
================
Host_ClearMemory

This clears all the memory used by both the client and server, but does
not reinitialize anything.
================
*/
void Host_ClearMemory()
{
	mpLogger->DPrintf("Clearing memory\n");
	
	D_FlushCaches();
	Mod_ClearAll();
	
	if(host_hunklevel)
		Hunk_FreeToLowMark(host_hunklevel);

	cls.signon = 0;
	
	memset(&sv, 0, sizeof(sv));
	memset(&cl, 0, sizeof(cl));
}

//============================================================================

/*
===================
Host_FilterTime

Returns false if the time is too short to run a frame
===================
*/
bool Host_FilterTime(float time)
{
	realtime += time;

	if(!cls.timedemo && realtime - oldrealtime < 1.0f / 72.0f)
		return false; // framerate is too high

	host_frametime = realtime - oldrealtime;
	oldrealtime = realtime;

	if(host_framerate.value > 0)
		host_frametime = host_framerate.value;
	else
	{
		// don't allow really long or short frames
		if(host_frametime > 0.1f)
			host_frametime = 0.1f;
		
		if(host_frametime < 0.001f)
			host_frametime = 0.001f;
	};

	return true;
}

/*
===================
Host_GetConsoleCommands

Add them exactly as if they had been typed at the console
===================
*/
void Host_GetConsoleCommands()
{
	char *cmd{nullptr};

	while(1)
	{
		cmd = Sys_ConsoleInput();
		
		if(!cmd)
			break;
		
		mpCmdBuffer->AddText(cmd);
	};
}

/*
==================
Host_ServerFrame

==================
*/
#ifdef FPS_20

void _Host_ServerFrame()
{
	// run the world state
	pr_global_struct->frametime = host_frametime;

	// read client messages
	SV_RunClients();

	// move things around and think
	// always pause in single player if in console or menus
	if(!sv.paused && (svs.maxclients > 1 || key_dest == key_game))
		SV_Physics();
}

void Host_ServerFrame()
{
	float save_host_frametime;
	float temp_host_frametime;

	// run the world state
	pr_global_struct->frametime = host_frametime;

	// set the time and clear the general datagram
	SV_ClearDatagram();

	// check for new clients
	SV_CheckForNewClients();

	temp_host_frametime = save_host_frametime = host_frametime;
	while(temp_host_frametime > (1.0 / 72.0))
	{
		if(temp_host_frametime > 0.05)
			host_frametime = 0.05;
		else
			host_frametime = temp_host_frametime;
		temp_host_frametime -= host_frametime;
		_Host_ServerFrame();
	}
	host_frametime = save_host_frametime;

	// send all messages to the clients
	SV_SendClientMessages();
}

#else

void Host_ServerFrame()
{
	// run the world state
	pr_global_struct->frametime = host_frametime;

	// set the time and clear the general datagram
	SV_ClearDatagram();

	// check for new clients
	SV_CheckForNewClients();

	// read client messages
	SV_RunClients();

	// move things around and think
	// always pause in single player if in console or menus
	if(!sv.paused && (svs.maxclients > 1 || key_dest == key_game))
		SV_Physics();

	// send all messages to the clients
	SV_SendClientMessages();
}

#endif


void _Host_Frame(float time)
{
	

	NET_Poll();

	// if running the server locally, make intentions now
	if(sv.active)
		CL_SendCmd();

	//-------------------
	//
	// server operations
	//
	//-------------------

	// check for commands typed to the host
	Host_GetConsoleCommands();

	if(sv.active)
		Host_ServerFrame();

	//-------------------
	//
	// client operations
	//
	//-------------------

	// if running the server remotely, send intentions now after
	// the incoming messages have been read
	if(!sv.active)
		CL_SendCmd();

	host_time += host_frametime;

	// fetch results from server
	if(cls.state == ca_connected)
		CL_ReadPackets();

	// update video
	if(host_speeds.value)
		time1 = Sys_FloatTime();

	SCR_UpdateScreen();

	if(host_speeds.value)
		time2 = Sys_FloatTime();

	// update audio
	if(cls.signon == SIGNONS)
	{
		S_SetListenerPos(r_origin, vpn, vright, vup);
		CL_DecayLights();
	}
	else
		S_SetListenerPos(vec3_origin, vec3_origin, vec3_origin, vec3_origin);

	S_Update();
	
	CDAudio_Update();

	if(host_speeds.value)
	{
		pass1 = (time1 - time3) * 1000;
		time3 = Sys_FloatTime();
		pass2 = (time2 - time1) * 1000;
		pass3 = (time3 - time2) * 1000;
		
		mpLogger->Printf("%3i tot %3i server %3i gfx %3i snd\n",
		           pass1 + pass2 + pass3, pass1, pass2, pass3);
	}

	host_framecount++;
}

//============================================================================

extern int vcrFile;
#define VCR_SIGNATURE 0x56435231
// "VCR1"

void Host_InitVCR(quakeparms_t *parms)
{
	int i, len, n;
	char *p;

	if(mpCmdLine->CheckParm("-playback"))
	{
		if(com_argc != 2)
			Sys_Error("No other parameters allowed with -playback\n");

		Sys_FileOpenRead("quake.vcr", &vcrFile);
		
		if(vcrFile == -1)
			Sys_Error("playback file not found\n");

		Sys_FileRead(vcrFile, &i, sizeof(int));
		
		if(i != VCR_SIGNATURE)
			Sys_Error("Invalid signature in vcr file\n");

		Sys_FileRead(vcrFile, &com_argc, sizeof(int));
		
		com_argv = malloc(com_argc * sizeof(char *));
		com_argv[0] = parms->argv[0];
		
		for(i = 0; i < com_argc; i++)
		{
			Sys_FileRead(vcrFile, &len, sizeof(int));
			p = malloc(len);
			Sys_FileRead(vcrFile, p, len);
			com_argv[i + 1] = p;
		};
		
		com_argc++; /* add one for arg[0] */
		
		parms->argc = com_argc;
		parms->argv = com_argv;
	}

	if((n = mpCmdLine->CheckParm("-record")) != 0)
	{
		vcrFile = Sys_FileOpenWrite("quake.vcr");

		i = VCR_SIGNATURE;
		Sys_FileWrite(vcrFile, &i, sizeof(int));
		i = com_argc - 1;
		Sys_FileWrite(vcrFile, &i, sizeof(int));
		for(i = 1; i < com_argc; i++)
		{
			if(i == n)
			{
				len = 10;
				Sys_FileWrite(vcrFile, &len, sizeof(int));
				Sys_FileWrite(vcrFile, "-playback", len);
				continue;
			};
			
			len = Q_strlen(com_argv[i]) + 1;
			
			Sys_FileWrite(vcrFile, &len, sizeof(int));
			Sys_FileWrite(vcrFile, com_argv[i], len);
		}
	}
}
