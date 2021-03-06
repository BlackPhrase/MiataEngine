/*
Copyright (C) 1996-1997 Id Software, Inc.
Copyright (C) 2019-2020 BlackPhrase

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

#include "quakedef.h"
#include "Host.hpp"

/*
====================
Host_Init
====================
*/
void CHost::Init(quakeparms_t *parms)
{
	if(standard_quake)
		minimum_memory = MINIMUM_MEMORY;
	else
		minimum_memory = MINIMUM_MEMORY_LEVELPAK;

	if(COM_CheckParm("-minmemory"))
		parms->memsize = minimum_memory;

	host_parms = *parms;

	if(parms->memsize < minimum_memory)
		mpSys->Error("Only %4.1f megs of memory available, can't execute game", parms->memsize / (float)0x100000);

	com_argc = parms->argc;
	com_argv = parms->argv;

	mpMemory->Init(parms->membase, parms->memsize);
	mpCbuf->Init();
	mpCmd->Init();
	
	//Host_InitVCR(parms);
	
	COM_Init();
	
	InitLocal();
	
	mpNet->Init();
	Netchan_Init();
	
	W_LoadWadFile("gfx.wad");
	Key_Init();
	mpConsole->Init();
	
	mpProgs->Init();
	mpModelCache->Init();
	
	mpServer->Init();
	
	mpConsole->Printf("Exe: " __TIME__ " " __DATE__ "\n");
	mpConsole->Printf("%4.1f megabyte heap\n", parms->memsize / (1024 * 1024.0));
	
	R_InitTextures(); // needed even for dedicated servers
	
	mpClient->Init();
	
	mpCbuf->InsertText("exec quake.rc\n");

	Hunk_AllocName(0, "-HOST_HUNKLEVEL-");
	host_hunklevel = Hunk_LowMark();

	host_initialized = true;
	
	mpSys->Printf("========Quake Initialized=========\n");
};

/*
===============
Host_Shutdown

FIXME: this is a callback from Sys_Quit and Sys_Error.  It would be better
to run quit through here before the final handoff to the sys code.
===============
*/
void CHost::Shutdown()
{
	static bool isdown{false};
	
	if(isdown)
	{
		printf("recursive shutdown\n");
		return;
	};
	
	isdown = true;
	
	// keep Con_Printf from trying to update the screen
	mpScreen->disabled_for_loading = true;

	mpClient->Shutdown();
	mpServer->Shutdown();
	
	mpNet->Shutdown();
};

void CHost::InitCommands()
{
	Host_InitCommands();
};

void CHost::ShutdownServer(bool crash)
{
	Host_ShutdownServer(crash);
};

/*
================
Host_ClearMemory

This clears all the memory used by both the client and server, but does
not reinitialize anything.
================
*/
void CHost::ClearMemory()
{
	mpConsole->DPrintf("Clearing memory\n");
	D_FlushCaches();
	Mod_ClearAll();
	
	if(host_hunklevel)
		Hunk_FreeToLowMark(host_hunklevel);

	cls.signon = 0;
	
	memset(&sv, 0, sizeof(sv));
	memset(&cl, 0, sizeof(cl));
};

void CHost::Frame(float time)
{
	double time1, time2;
	static double timetotal;
	static int timecount;
	int i, c, m;

	if(!serverprofile.value)
	{
		_Host_Frame(time);
		return;
	};
	
	time1 = Sys_DoubleTime();
	_Host_Frame(time);
	time2 = Sys_DoubleTime();	
	
	timetotal += time2 - time1;
	timecount++;
	
	if (timecount < 1000)
		return;

	m = timetotal*1000/timecount;
	timecount = 0;
	timetotal = 0;
	c = 0;
	
	for(i = 0; i < svs.maxclients; ++i)
	{
		if (svs.clients[i].active)
			c++;
	};

	mpConsole->Printf("serverprofile: %2i clients %2i msec\n",  c,  m);
};

void CHost::ServerFrame()
{
	Host_ServerFrame();
};

/*
================
Host_Error

This shuts down both the client and server
================
*/
void CHost::Error(const char *error, ...)
{
	va_list argptr;
	char string[1024]{};
	static bool inerror{false};
	
	if(inerror)
		Sys_Error("Host_Error: recursively entered");
	inerror = true;
	
	mpScreen->EndLoadingPlaque(); // reenable screen updates

	va_start(argptr, error);
	vsprintf(string, error, argptr);
	va_end(argptr);
	
	Con_Printf("Host_Error: %s\n", string);
	
	if(sv.active)
		ShutdownServer(false);

	if(mpClient->state == ca_dedicated)
		mpSys->Error("Host_Error: %s\n", string); // dedicated servers exit

	mpClient->Disconnect();
	cls.demonum = -1;

	inerror = false;

	longjmp(host_abortframe, 1);
};

/*
================
Host_EndGame
================
*/
void CHost::EndGame(const char *message, ...)
{
	va_list argptr;
	char string[1024]{};
	
	va_start(argptr, message);
	vsprintf(string, message, argptr);
	va_end(argptr);
	
	mpConsole->DPrintf("Host_EndGame: %s\n", string);
	
	if(sv.active)
		ShutdownServer(false);

	if(mpClient->state == ca_dedicated)
		mpSys->Error("Host_EndGame: %s\n", string); // dedicated servers exit
	
	if(cls.demonum != -1)
		CL_NextDemo();
	else
		mpClient->Disconnect();

	longjmp(host_abortframe, 1);
};

void CHost::ClientCommands(const char *fmt, ...)
{
	Host_ClientCommands(fmt);
};

void Host_Quit_f()
{
};

/*
=======================
Host_InitLocal
======================
*/
void CHost::InitLocal()
{
	InitCommands();
	
	Cvar_RegisterVariable (&host_framerate);
	Cvar_RegisterVariable (&host_speeds);

	Cvar_RegisterVariable (&sys_ticrate);
	Cvar_RegisterVariable (&serverprofile);

	Cvar_RegisterVariable (&fraglimit);
	Cvar_RegisterVariable (&timelimit);
	Cvar_RegisterVariable (&teamplay);
	Cvar_RegisterVariable (&samelevel);
	Cvar_RegisterVariable (&noexit);
	Cvar_RegisterVariable (&skill);
	Cvar_RegisterVariable (&developer);
	Cvar_RegisterVariable (&deathmatch);
	Cvar_RegisterVariable (&coop);

	Cvar_RegisterVariable (&pausable);

	Cvar_RegisterVariable (&temp1);

	Host_FindMaxClients();
	
	host_time = 1.0; // so a think at time 0 won't get called
};

/*
==================
Host_Frame

Runs all active servers
==================
*/
void CHost::_Frame(float time)
{
	static double		time1 = 0;
	static double		time2 = 0;
	static double		time3 = 0;
	int			pass1, pass2, pass3;

	if (setjmp (host_abortframe) )
		return;			// something bad happened, or the server disconnected

// keep the random time dependent
	rand ();
	
// decide the simulation time
	if (!Host_FilterTime(time))
		return;			// don't run too fast, or packets will flood out

// if running the server locally, make intentions now
	if (sv.active)
		CL_SendCmd ();
	
//-------------------
//
// server operations
//
//-------------------

	// check for commands typed to the host
	Host_GetConsoleCommands();
	
	if(mpServer->active)
		mpServer->Frame(time);

//-------------------
//
// client operations
//
//-------------------

	mpClient->Frame(time);
};