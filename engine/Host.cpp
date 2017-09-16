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

#include <cstdio>
#include "Host.hpp"
#include "ModuleLoader.hpp"
#include "Logger.hpp"
#include "EngineInterface.hpp"
#include "CvarList.hpp"
#include "CvarController.hpp"
#include "CvarDispatcher.hpp"
#include "CmdList.hpp"
#include "CmdExecutor.hpp"
#include "CmdBuffer.hpp"
#include "CmdArgs.hpp"
#include "IGame.hpp"
#include "IClientGame.hpp"
#include "IMenu.hpp"
#include "IInput.hpp"
#include "ISound.hpp"
#include "INetwork.hpp"
#include "IVideo.hpp"

void Sys_Quit()
{
	exit(-1);
};

static void DoABarrelRoll(const CCmdArgs &aArgs)
{
	printf("Barrel roll!\n");
	system("pause");
};

CHost::CHost() = default;
CHost::~CHost() = default;

/*
====================
Host_Init
====================
*/
bool CHost::Init(quakeparms_t *parms)
{
	// Already done
	if(host_initialized)
		return true;
	
	//minimum_memory = MINIMUM_MEMORY;

	//if(mpCmdLine->CheckParm("-minmemory"))
		//parms->memsize = minimum_memory;

	host_parms = *parms;

	//if(parms->memsize < minimum_memory)
		//Sys_Error("Only %4.1f megs of memory available, can't execute game", parms->memsize / (float)0x100000);

	//mpCmdLine = std::make_unique<CCmdArgs>(parms->argc, parms->argv);
	
	mpModuleLoader = std::make_unique<CModuleLoader>();
	mpLogger = std::make_unique<CLogger>();
	mpEngineInterface = std::make_unique<CEngineInterface>(mpLogger.get());
	
	mpCvarDispatcher = std::make_unique<CCvarDispatcher>();
	mpCvarList = std::make_unique<CCvarList>(mpCvarDispatcher.get());
	mpCmdList = std::make_unique<CCmdList>();
	mpCvarController = std::make_unique<CCvarController>(mpCvarList.get(), mpCmdList.get());
	mpCmdExecutor = std::make_unique<CCmdExecutor>(mpCvarList.get(), mpCmdList.get());
	mpCmdBuffer = std::make_unique<CCmdBuffer>(mpLogger.get(), mpCmdExecutor.get());
	
	mpCvarList->Create("developer", "1");
	
	mpCmdList->Add("barrelroll", DoABarrelRoll, "Do a barrel roll!");
	
	/*
	mpCmdExecutor->ExecString("test blah");
	mpCmdExecutor->ExecString("test;blah");
	mpCmdExecutor->ExecString("test blah;");
	mpCmdExecutor->ExecString(";test blah "); // TODO: strip
	mpCmdExecutor->ExecString("test ");
	mpCmdExecutor->ExecString("test");
	*/
	mpCmdExecutor->ExecString("developer", src_client);
	mpCmdExecutor->ExecString("developer 2", src_client);
	mpCmdExecutor->ExecString("developer", src_client);
	mpCmdExecutor->ExecString("barrelroll", src_client);
	mpCmdExecutor->ExecString("quit", src_client);
	
	//Memory_Init(parms->membase, parms->memsize);
	
	mpCmdBuffer->Init();
	
	/*
	Cmd_Init();
	
	V_Init();
	Chase_Init();
	
	Host_InitVCR(parms);
	
	COM_Init(parms->basedir);
	Host_InitLocal();
	
	W_LoadWadFile("gfx.wad");
	Con_Init();
	*/
	
	//PR_Init();
	//Mod_Init();
	
	mpNetwork = mpModuleLoader->LoadModule<INetwork>("network", "GetNetwork");
	mpNetwork->Init();
	
	//SV_Init();
	
	mpGame = mpModuleLoader->LoadModule<IGame>("game", "GetGame");
	mpGame->Init();

	mpLogger->Printf("Exe: " __TIME__ " " __DATE__ "\n");
	mpLogger->Printf("%4.1f megabyte heap\n", parms->memsize / (1024 * 1024.0));

	//R_InitTextures(); // needed even for dedicated servers

	//if(cls.state != ca_dedicated)
	if(!host_parms.dedicated)
	{
		//host_basepal = (byte *)COM_LoadHunkFile("gfx/palette.lmp");
		//if(!host_basepal)
			//Sys_Error("Couldn't load gfx/palette.lmp");
		//host_colormap = (byte *)COM_LoadHunkFile("gfx/colormap.lmp");
		//if(!host_colormap)
			//Sys_Error("Couldn't load gfx/colormap.lmp");

		mpInput = mpModuleLoader->LoadModule<IInput>("input", "GetInput");
		
#ifndef _WIN32 // on non win32, mouse comes before video for security reasons
		mpInput->Init();
#endif
		
		mpVideo = mpModuleLoader->LoadModule<IVideo>("video", "GetVideo");
		mpVideo->Init(/*host_basepal*/);

		//Draw_Init();
		//SCR_Init();
		//R_Init();
		
		// on Win32, sound initialization has to come before video initialization, so we
		// can put up a popup if the sound hardware is in use
		//if(!mpCmdLine->CheckParm("-nosound"))
		{
			mpSound = mpModuleLoader->LoadModule<ISound>("sound", "GetSound");
			mpSound->Init();
		};
		
		//mpCDAudio->Init();
		//Sbar_Init();
		//CL_Init();
		
#ifdef _WIN32 // on non win32, mouse comes before video for security reasons
		mpInput->Init();
#endif
		
		//Key_Init();
		
		mpMenu = mpModuleLoader->LoadModule<IMenu>("menu", "GetMenu");
		mpMenu->Init();
		
		mpClGame = mpModuleLoader->LoadModule<IClientGame>("clgame", "GetClientGame");
		mpClGame->Init();
	};

	mpCmdBuffer->InsertText("exec quake.rc\n");

	//Hunk_AllocName(0, "-HOST_HUNKLEVEL-");
	//host_hunklevel = Hunk_LowMark();

	host_initialized = true;
	
	system("pause");

	//Sys_Printf("========Quake (what's rest of it) Initialized=========\n");
	return true;
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
	// That's not funny...
	if(!host_initialized)
		return;
	
	static bool bDown{false};
	
	if(bDown)
	{
		printf("recursive shutdown\n");
		return;
	};
	
	bDown = true;
	
	mpGame->Shutdown();
	
	// TODO: check for dedicated
	//if(cls.state != ca_dedicated)
	if(!host_parms.dedicated)
	{
		/*
		// keep Con_Printf from trying to update the screen
		scr_disabled_for_loading = true;

		Host_WriteConfig();

		mpCDAudio->Shutdown();
		*/
		
		mpClGame->Shutdown();
		mpMenu->Shutdown();
		
		mpSound->Shutdown();
		mpInput->Shutdown();
		
		mpVideo->Shutdown();
	};
	
	mpNetwork->Shutdown();
	
	Sys_Quit();
};

void CHost::Frame()
{
	// Stop breaking the system!
	if(!host_initialized)
		return;

	double time1 = 0.0; //Sys_FloatTime();
	Update(0.1);
	double time2 = 0.0; //Sys_FloatTime();
	
	Render();

	//if(serverprofile.value)
	{
		static double timetotal{0.0};
		static int timecount{0};
		
		timetotal += time2 - time1;
		timecount++;

		if(timecount < 1000)
			return;

		int m = timetotal * 1000 / timecount;
		timecount = 0;
		timetotal = 0;
		int c{0};
		
		//for(int i = 0; i < svs.maxclients; i++)
		{
			//if(svs.clients[i].active)
				//c++;
		};

		mpLogger->Printf("serverprofile: %2i clients %2i msec\n", c, m);
	};
	
	static int tempframe{0};
	++tempframe;
	
	if(tempframe >= 10000)
		Shutdown();
};

/*
==================
Host_Frame

Runs all active servers
==================
*/
void CHost::Update(double frametime)
{
	static double time1{0.0};
	static double time2{0.0};
	static double time3{0.0};
	int pass1{0}, pass2{0}, pass3{0};

	//if(setjmp(host_abortserver))
		//return; // something bad happened, or the server disconnected

	// keep the random time dependent
	//rand();

	// decide the simulation time
	//if(!Host_FilterTime(time))
		//return; // don't run too fast, or packets will flood out

	// get new key events
	//Sys_SendKeyEvents();

	// allow mice or other external controllers to add commands
	//IN_Commands();

	// process console commands
	mpCmdBuffer->Execute();
	
	mpNetwork->Frame();
	mpGame->Frame();
	
	if(!host_parms.dedicated)
	{
		mpMenu->Frame();
		mpSound->Frame();
		mpInput->Frame();
		mpClGame->Frame();
		
		mpVideo->CheckChanges();
	};
};

void CHost::Render()
{
};