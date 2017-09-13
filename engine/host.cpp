#include <cstdio>
#include "Host.hpp"
#include "ModuleLoader.hpp"
#include "CvarList.hpp"
#include "CvarManager.hpp"
#include "CmdList.hpp"
#include "CmdExecutor.hpp"
#include "CmdBuffer.hpp"
#include "IGame.hpp"
#include "IClientGame.hpp"
#include "IMenu.hpp"
#include "IInput.hpp"
#include "ISound.hpp"
#include "INetwork.hpp"

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
bool CHost::Init(/*quakeparms_t *parms*/)
{
	// Already done
	if(host_initialized)
		return true;
	
	//minimum_memory = MINIMUM_MEMORY;

	//if(COM_CheckParm("-minmemory"))
		//parms->memsize = minimum_memory;

	//host_parms = *parms;

	//if(parms->memsize < minimum_memory)
		//Sys_Error("Only %4.1f megs of memory available, can't execute game", parms->memsize / (float)0x100000);

	//com_argc = parms->argc;
	//com_argv = parms->argv;
	
	mpModuleLoader = std::make_unique<CModuleLoader>();
	
	mpCvarList = std::make_unique<CCvarList>();
	mpCmdList = std::make_unique<CCmdList>();
	mpCvarManager = std::make_unique<CCvarManager>(mpCvarList.get(), mpCmdList.get());
	mpCmdExecutor = std::make_unique<CCmdExecutor>(mpCvarList.get(), mpCmdList.get(), mpCvarManager.get());
	mpCmdBuffer = std::make_unique<CCmdBuffer>(mpCmdExecutor.get());
	
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
	
	mpGame = mpModuleLoader->LoadModule<IGame>("game", "GetGame");
	mpGame->Init();
	
	mpMenu = mpModuleLoader->LoadModule<IMenu>("menu", "GetMenu");
	mpMenu->Init();
	
	mpClGame = mpModuleLoader->LoadModule<IClientGame>("clgame", "GetClientGame");
	mpClGame->Init();
	
	mpInput = mpModuleLoader->LoadModule<IInput>("input", "GetInput");
	mpInput->Init();
	//Key_Init();
	
	// TODO: check for "-nosound"
	mpSound = mpModuleLoader->LoadModule<ISound>("sound", "GetSound");
	mpSound->Init();
	
	// TODO: video
	
	//PR_Init();
	//Mod_Init();
	
	mpNetwork = mpModuleLoader->LoadModule<INetwork>("network", "GetNetwork");
	mpNetwork->Init();
	
	/*
	SV_Init();

	Con_Printf("Exe: " __TIME__ " " __DATE__ "\n");
	
	Con_Printf("%4.1f megabyte heap\n", parms->memsize / (1024 * 1024.0));

	R_InitTextures(); // needed even for dedicated servers

	if(cls.state != ca_dedicated)
	{
		host_basepal = (byte *)COM_LoadHunkFile("gfx/palette.lmp");
		if(!host_basepal)
			Sys_Error("Couldn't load gfx/palette.lmp");
		host_colormap = (byte *)COM_LoadHunkFile("gfx/colormap.lmp");
		if(!host_colormap)
			Sys_Error("Couldn't load gfx/colormap.lmp");

#ifndef _WIN32 // on non win32, mouse comes before video for security reasons
		IN_Init();
#endif
		VID_Init(host_basepal);

		Draw_Init();
		SCR_Init();
		R_Init();
#ifndef _WIN32
		// on Win32, sound initialization has to come before video initialization, so we
		// can put up a popup if the sound hardware is in use
		S_Init();
#else

#ifdef GLQUAKE
		// FIXME: doesn't use the new one-window approach yet
		S_Init();
#endif

#endif // _WIN32
		CDAudio_Init();
		Sbar_Init();
		CL_Init();
		
#ifdef _WIN32 // on non win32, mouse comes before video for security reasons
		IN_Init();
#endif
	};
	*/

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
	
	static bool isdown{false};
	
	if(isdown)
	{
		printf("recursive shutdown\n");
		return;
	};
	
	isdown = true;
	
	mpGame->Shutdown();
	mpClGame->Shutdown();
	mpMenu->Shutdown();

	/*
	// keep Con_Printf from trying to update the screen
	scr_disabled_for_loading = true;

	Host_WriteConfiguration();

	CDAudio_Shutdown();
	*/
	
	mpNetwork->Shutdown();
	mpSound->Shutdown();
	mpInput->Shutdown();
	
	//if(cls.state != ca_dedicated)
		//VID_Shutdown();
	
	//Sys_Quit();
};

void CHost::Frame(double time) // was float
{
	// Stop breaking the system!
	if(!host_initialized)
		return;
	
	double time1{0.0}, time2{0.0};
	static double timetotal{0.0};
	static int timecount{0};
	int i{0}, c{0}, m{0};

	//if(!serverprofile.value)
	{
		//_Frame(time);
		//return;
	};

	//time1 = Sys_FloatTime();
	_Frame(time);
	//time2 = Sys_FloatTime();

	timetotal += time2 - time1;
	timecount++;

	if(timecount < 1000)
		return;

	m = timetotal * 1000 / timecount;
	timecount = 0;
	timetotal = 0;
	c = 0;
	
	//for(i = 0; i < svs.maxclients; i++)
	{
		//if(svs.clients[i].active)
			c++;
	};

	//Con_Printf("serverprofile: %2i clients %2i msec\n", c, m);
};

/*
==================
Host_Frame

Runs all active servers
==================
*/
void CHost::_Frame(double frametime)
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
	
	mpMenu->Frame();
	mpSound->Frame();
	mpInput->Frame();
	mpClGame->Frame();
};