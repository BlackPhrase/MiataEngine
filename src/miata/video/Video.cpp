/*
Copyright (C) 1997-2001 Id Software, Inc.
Copyright (C) 2017-2018 BlackPhrase

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

/// @file

#include "Video.hpp"

CVideo::CVideo(IEngineInterface *apEngine) : mpEngine(apEngine){}
CVideo::~CVideo() = default;

/*
============
VID_Init
============
*/
bool CVideo::Init()
{
	mpEngine->GetLogger()->Printf("Video initialization...");
	
	// Create the video variables so we know how to start the graphics drivers
	vid_ref = mpEngine->GetCvarList()->Register("vid_ref", "gl", CVAR_ARCHIVE);
	vid_xpos = mpEngine->GetCvarList()->Register("vid_xpos", "3", CVAR_ARCHIVE);
	vid_ypos = mpEngine->GetCvarList()->Register("vid_ypos", "22", CVAR_ARCHIVE);
	vid_fullscreen = mpEngine->GetCvarList()->Register("vid_fullscreen", "0", CVAR_ARCHIVE);
	vid_gamma = mpEngine->GetCvarList()->Register( "vid_gamma", "1", CVAR_ARCHIVE );
	win_noalttab = mpEngine->GetCvarList()->Register( "win_noalttab", "0", CVAR_ARCHIVE );

	// Add some console commands that we want to handle
	mpEngine->GetCmdList()->AddCommand ("vid_restart", VID_Restart_f);
	mpEngine->GetCmdList()->AddCommand ("vid_front", VID_Front_f);

	/*
	** this is a gross hack but necessary to clamp the mode for 3Dfx
	*/
#if 0
	{
		cvar_t *gl_driver = mpEngine->GetCvarList()->Register( "gl_driver", "opengl32", 0 );
		cvar_t *gl_mode = mpEngine->GetCvarList()->Register( "gl_mode", "3", 0 );

		if ( stricmp( gl_driver->string, "3dfxgl" ) == 0 )
		{
			Cvar_SetValue( "gl_mode", 3 );
			viddef.width  = 640;
			viddef.height = 480;
		}
	}
#endif

	// Disable the 3Dfx splash screen
	putenv("FX_GLIDE_NO_SPLASH=0");
		
	// Start the graphics mode and load render DLL
	CheckChanges();
	
	return true;
};

/*
============
VID_Shutdown
============
*/
void CVideo::Shutdown()
{
	mpEngine->GetLogger()->Printf("Video shutdown...");
	
	if(reflib_active)
	{
		re.Shutdown();
		FreeRenderModule();
	};
};

//void CVideo::Frame()
//{
//};

/*
============
VID_CheckChanges

This function gets called once just before drawing each frame, and it's sole purpose in life
is to check to see if any of the video mode parameters have changed, and if they have to 
update the rendering DLL and/or video mode to match.
============
*/
void CVideo::CheckChanges()
{
	char name[100]{};

	if(vid_ref->modified)
	{
		cl.force_refdef = true; // can't use a paused refdef
		S_StopAllSounds();
	};
	
	while(vid_ref->modified)
	{
		/*
		** refresh has changed
		*/
		vid_ref->modified = false;
		vid_fullscreen->modified = true;
		cl.refresh_prepped = false;
		cls.disable_screen = true;

		Com_sprintf( name, sizeof(name), "r_%s", vid_ref->string );
		
		if ( !LoadRenderModule( name ) )
		{
			if ( strcmp (vid_ref->string, "soft") == 0 )
				Com_Error (ERR_FATAL, "Couldn't fall back to software refresh!");
			Cvar_Set( "vid_ref", "soft" );

			/*
			** drop the console if we fail to load a refresh
			*/
			if ( cls.key_dest != key_console )
				Con_ToggleConsole_f();
		};
		cls.disable_screen = false;
	};

	/*
	** update our window position
	*/
	if ( vid_xpos->modified || vid_ypos->modified )
	{
		if (!vid_fullscreen->value)
			VID_UpdateWindowPosAndSize( vid_xpos->value, vid_ypos->value );

		vid_xpos->modified = false;
		vid_ypos->modified = false;
	};
};

/*
==============
VID_LoadRefresh
==============
*/
bool CVideo::LoadRenderModule(const char *name)
{
	refimport_t	ri;
	GetRefAPI_t	GetRefAPI;
	
	if(reflib_active)
	{
		re.Shutdown();
		FreeRenderModule();
	};

	Com_Printf("------- Loading %s -------\n", name);

	reflib_library = Sys_LoadLibrary(name);
	
	if(!reflib_library)
	{
		Com_Printf("LoadLibrary(\"%s\") failed\n", name);
		return false;
	};

	ri.Cmd_AddCommand = Cmd_AddCommand;
	ri.Cmd_RemoveCommand = Cmd_RemoveCommand;
	ri.Cmd_Argc = Cmd_Argc;
	ri.Cmd_Argv = Cmd_Argv;
	ri.Cmd_ExecuteText = Cbuf_ExecuteText;
	ri.Con_Printf = VID_Printf;
	ri.Sys_Error = VID_Error;
	ri.FS_LoadFile = FS_LoadFile;
	ri.FS_FreeFile = FS_FreeFile;
	ri.FS_Gamedir = FS_Gamedir;
	ri.Cvar_Get = Cvar_Get;
	ri.Cvar_Set = Cvar_Set;
	ri.Cvar_SetValue = Cvar_SetValue;
	ri.Vid_GetModeInfo = VID_GetModeInfo;
	ri.Vid_MenuInit = VID_MenuInit;
	ri.Vid_NewWindow = VID_NewWindow;

	GetRefAPI = (void *)Sys_GetProcAddress(reflib_library, "GetRefAPI");
	
	if(!GetRefAPI)
		Com_Error(ERR_FATAL, "GetProcAddress failed on %s", name);

	re = GetRefAPI(ri);

	if(re.api_version != API_VERSION)
	{
		FreeRenderModule();
		Com_Error(ERR_FATAL, "%s has incompatible api_version", name);
	};

	if(re.Init(global_hInstance, MainWndProc) == -1)
	{
		re.Shutdown();
		FreeRenderModule();
		return false;
	};

	Com_Printf( "------------------------------------\n");
	reflib_active = true;

//======
//PGM
	vidref_val = VIDREF_OTHER;
	if(vid_ref)
	{
		if(!strcmp (vid_ref->string, "gl"))
			vidref_val = VIDREF_GL;
		else if(!strcmp(vid_ref->string, "soft"))
			vidref_val = VIDREF_SOFT;
	};
//PGM
//======

	return true;
};

void CVideo::FreeRenderModule()
{
	if(!Sys_FreeLibrary(reflib_library))
		Com_Error(ERR_FATAL, "Render FreeLibrary failed");
	
	memset(&re, 0, sizeof(re));
	
	reflib_library = nullptr;
	reflib_active = false;
};