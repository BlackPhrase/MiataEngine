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

// vid_null.cpp -- null video driver to aid porting efforts

#include "quakedef.hpp"
#include "d_local.hpp"

viddef_t vid; // global video state

#define BASEWIDTH 320
#define BASEHEIGHT 200

byte vid_buffer[BASEWIDTH * BASEHEIGHT];
short zbuffer[BASEWIDTH * BASEHEIGHT];
byte surfcache[256 * 1024];

unsigned short d_8to16table[256];
unsigned d_8to24table[256];

void VID_SetPalette(byte *palette)
{
}

void VID_ShiftPalette(byte *palette)
{
}

void VID_Init(byte *palette)
{
	vid.maxwarpwidth = vid.width = vid.conwidth = BASEWIDTH;
	vid.maxwarpheight = vid.height = vid.conheight = BASEHEIGHT;
	vid.aspect = 1.0;
	vid.numpages = 1;
	vid.colormap = host_colormap;
	vid.fullbright = 256 - LittleLong(*((int *)vid.colormap + 2048));
	vid.buffer = vid.conbuffer = vid_buffer;
	vid.rowbytes = vid.conrowbytes = BASEWIDTH;

	d_pzbuffer = zbuffer;
	D_InitCaches(surfcache, sizeof(surfcache));
}

void VID_Update(vrect_t *rects){};

/*
================
D_BeginDirectRect
================
*/
void D_BeginDirectRect(int x, int y, byte *pbitmap, int width, int height){};

/*
================
D_EndDirectRect
================
*/
void D_EndDirectRect(int x, int y, int width, int height){};

void CVideoNull::Init()
{
    refimport_t	ri;

    viddef.width = 320;
    viddef.height = 240;

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
	ri.Vid_NewWindow = VID_NewWindow;
    ri.Cvar_Get = Cvar_Get;
    ri.Cvar_Set = Cvar_Set;
    ri.Cvar_SetValue = Cvar_SetValue;
    ri.Vid_GetModeInfo = VID_GetModeInfo;

    re = GetRefAPI(ri);

    if(re.api_version != API_VERSION)
        Com_Error(ERR_FATAL, "Re has incompatible api_version");
    
	// call the init function
    if(re.Init(nullptr, nullptr) == -1)
		Com_Error(ERR_FATAL, "Couldn't start refresh");
};

void CVideoNull::Shutdown()
{
    if(re.Shutdown)
	    re.Shutdown();
};

void CVideoNull::CheckChanges()
{
};