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
// gl_vidnt.c -- NT GL vid component

#include "quakedef.hpp"
#include "winquake.hpp"
#include "resource.hpp"
#include <commctrl.h>

#define MAX_MODE_LIST 30
#define VID_ROW_SIZE 3
#define WARP_WIDTH 320
#define WARP_HEIGHT 200
#define MAXWIDTH 10000
#define MAXHEIGHT 10000
#define BASEWIDTH 320
#define BASEHEIGHT 200

#define MODE_WINDOWED 0
#define NO_MODE (MODE_WINDOWED - 1)
#define MODE_FULLSCREEN_DEFAULT (MODE_WINDOWED + 1)

typedef struct
{
	modestate_t type;
	int width;
	int height;
	int modenum;
	int dib;
	int fullscreen;
	int bpp;
	int halfscreen;
	char modedesc[17];
} vmode_t;

typedef struct
{
	int width;
	int height;
} lmode_t;

lmode_t lowresmodes[] = {
	{ 320, 200 },
	{ 320, 240 },
	{ 400, 300 },
	{ 512, 384 },
};

const char *gl_vendor;
const char *gl_renderer;
const char *gl_version;
const char *gl_extensions;

qboolean DDActive;
qboolean scr_skipupdate;

static vmode_t modelist[MAX_MODE_LIST];
static int nummodes;
static vmode_t *pcurrentmode;
static vmode_t badmode;

static DEVMODE gdevmode;
static qboolean vid_initialized = false;
static qboolean windowed, leavecurrentmode;
static qboolean vid_canalttab = false;
static qboolean vid_wassuspended = false;
static int windowed_mouse;
extern qboolean mouseactive; // from in_win.c
static HICON hIcon;

int DIBWidth, DIBHeight;
RECT WindowRect;
DWORD WindowStyle, ExWindowStyle;

HWND mainwindow, dibwindow;

int vid_modenum = NO_MODE;
int vid_realmode;
int vid_default = MODE_WINDOWED;
static int windowed_default;
unsigned char vid_curpal[256 * 3];
static qboolean fullsbardraw = false;

static float vid_gamma = 1.0;

HGLRC baseRC;
HDC maindc;

glvert_t glv;

cvar_t gl_ztrick = { "gl_ztrick", "1" };

viddef_t vid; // global video state

unsigned short d_8to16table[256];
unsigned d_8to24table[256];
unsigned char d_15to8table[65536];

float gldepthmin, gldepthmax;

modestate_t modestate = MS_UNINIT;

void VID_MenuDraw();
void VID_MenuKey(int key);

void AppActivate(BOOL fActive, BOOL minimize);
char *VID_GetModeDescription(int mode);
void ClearAllStates();
void VID_UpdateWindowStatus();

typedef void(APIENTRY *lp3DFXFUNC)(int, int, int, int, int, const void *);

qboolean is8bit = false;
qboolean isPermedia = false;


//====================================

cvar_t vid_mode = { "vid_mode", "0", false };
// Note that 0 is MODE_WINDOWED
cvar_t _vid_default_mode = { "_vid_default_mode", "0", true };
// Note that 3 is MODE_FULLSCREEN_DEFAULT
cvar_t _vid_default_mode_win = { "_vid_default_mode_win", "3", true };
cvar_t vid_wait = { "vid_wait", "0" };
cvar_t vid_nopageflip = { "vid_nopageflip", "0", true };
cvar_t _vid_wait_override = { "_vid_wait_override", "0", true };
cvar_t vid_config_x = { "vid_config_x", "800", true };
cvar_t vid_config_y = { "vid_config_y", "600", true };
cvar_t vid_stretch_by_2 = { "vid_stretch_by_2", "1", true };
cvar_t _windowed_mouse = { "_windowed_mouse", "1", true };

int window_center_x, window_center_y, window_x, window_y, window_width, window_height;
RECT window_rect;

// direct draw software compatability stuff

void VID_HandlePause(bool pause)
{
}

void VID_ForceLockState(int lk)
{
}

void VID_LockBuffer()
{
}

void VID_UnlockBuffer()
{
}

int VID_ForceUnlockedAndReturnState()
{
	return 0;
}

void D_BeginDirectRect(int x, int y, byte *pbitmap, int width, int height)
{
}

void D_EndDirectRect(int x, int y, int width, int height)
{
}

void CenterWindow(HWND hWndCenter, int width, int height, BOOL lefttopjustify)
{
	RECT rect;
	int CenterX, CenterY;

	CenterX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	CenterY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
	if(CenterX > CenterY * 2)
		CenterX >>= 1; // dual screens
	CenterX = (CenterX < 0) ? 0 : CenterX;
	CenterY = (CenterY < 0) ? 0 : CenterY;
	SetWindowPos(hWndCenter, NULL, CenterX, CenterY, 0, 0,
	             SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_DRAWFRAME);
}

qboolean VID_SetWindowedMode(int modenum)
{
	HDC hdc;
	int lastmodestate, width, height;
	RECT rect;

	lastmodestate = modestate;

	WindowRect.top = WindowRect.left = 0;

	WindowRect.right = modelist[modenum].width;
	WindowRect.bottom = modelist[modenum].height;

	DIBWidth = modelist[modenum].width;
	DIBHeight = modelist[modenum].height;

	WindowStyle = WS_OVERLAPPED | WS_BORDER | WS_CAPTION | WS_SYSMENU |
	WS_MINIMIZEBOX;
	ExWindowStyle = 0;

	rect = WindowRect;
	AdjustWindowRectEx(&rect, WindowStyle, FALSE, 0);

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	// Create the DIB window
	dibwindow = CreateWindowEx(
	ExWindowStyle,
	"WinQuake",
	"GLQuake",
	WindowStyle,
	rect.left, rect.top,
	width,
	height,
	NULL,
	NULL,
	global_hInstance,
	NULL);

	if(!dibwindow)
		Sys_Error("Couldn't create DIB window");

	// Center and show the DIB window
	CenterWindow(dibwindow, WindowRect.right - WindowRect.left,
	             WindowRect.bottom - WindowRect.top, false);

	ShowWindow(dibwindow, SW_SHOWDEFAULT);
	UpdateWindow(dibwindow);

	modestate = MS_WINDOWED;

	// because we have set the background brush for the window to NULL
	// (to avoid flickering when re-sizing the window on the desktop),
	// we clear the window to black when created, otherwise it will be
	// empty while Quake starts up.
	hdc = GetDC(dibwindow);
	PatBlt(hdc, 0, 0, WindowRect.right, WindowRect.bottom, BLACKNESS);
	ReleaseDC(dibwindow, hdc);

	if(vid.conheight > modelist[modenum].height)
		vid.conheight = modelist[modenum].height;
	if(vid.conwidth > modelist[modenum].width)
		vid.conwidth = modelist[modenum].width;
	vid.width = vid.conwidth;
	vid.height = vid.conheight;

	vid.numpages = 2;

	mainwindow = dibwindow;

	SendMessage(mainwindow, WM_SETICON, (WPARAM)TRUE, (LPARAM)hIcon);
	SendMessage(mainwindow, WM_SETICON, (WPARAM)FALSE, (LPARAM)hIcon);

	return true;
}

qboolean VID_SetFullDIBMode(int modenum)
{
	HDC hdc;
	int lastmodestate, width, height;
	RECT rect;

	if(!leavecurrentmode)
	{
		gdevmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		gdevmode.dmBitsPerPel = modelist[modenum].bpp;
		gdevmode.dmPelsWidth = modelist[modenum].width << modelist[modenum].halfscreen;
		gdevmode.dmPelsHeight = modelist[modenum].height;
		gdevmode.dmSize = sizeof(gdevmode);

		if(ChangeDisplaySettings(&gdevmode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			Sys_Error("Couldn't set fullscreen DIB mode");
	}

	lastmodestate = modestate;
	modestate = MS_FULLDIB;

	WindowRect.top = WindowRect.left = 0;

	WindowRect.right = modelist[modenum].width;
	WindowRect.bottom = modelist[modenum].height;

	DIBWidth = modelist[modenum].width;
	DIBHeight = modelist[modenum].height;

	WindowStyle = WS_POPUP;
	ExWindowStyle = 0;

	rect = WindowRect;
	AdjustWindowRectEx(&rect, WindowStyle, FALSE, 0);

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	// Create the DIB window
	dibwindow = CreateWindowEx(
	ExWindowStyle,
	"WinQuake",
	"GLQuake",
	WindowStyle,
	rect.left, rect.top,
	width,
	height,
	NULL,
	NULL,
	global_hInstance,
	NULL);

	if(!dibwindow)
		Sys_Error("Couldn't create DIB window");

	ShowWindow(dibwindow, SW_SHOWDEFAULT);
	UpdateWindow(dibwindow);

	// Because we have set the background brush for the window to NULL
	// (to avoid flickering when re-sizing the window on the desktop), we
	// clear the window to black when created, otherwise it will be
	// empty while Quake starts up.
	hdc = GetDC(dibwindow);
	PatBlt(hdc, 0, 0, WindowRect.right, WindowRect.bottom, BLACKNESS);
	ReleaseDC(dibwindow, hdc);

	if(vid.conheight > modelist[modenum].height)
		vid.conheight = modelist[modenum].height;
	if(vid.conwidth > modelist[modenum].width)
		vid.conwidth = modelist[modenum].width;
	vid.width = vid.conwidth;
	vid.height = vid.conheight;

	vid.numpages = 2;

	// needed because we're not getting WM_MOVE messages fullscreen on NT
	window_x = 0;
	window_y = 0;

	mainwindow = dibwindow;

	SendMessage(mainwindow, WM_SETICON, (WPARAM)TRUE, (LPARAM)hIcon);
	SendMessage(mainwindow, WM_SETICON, (WPARAM)FALSE, (LPARAM)hIcon);

	return true;
}

int VID_SetMode(int modenum, unsigned char *palette)
{
	int original_mode, temp;
	qboolean stat;
	MSG msg;
	HDC hdc;

	if((windowed && (modenum != 0)) ||
	   (!windowed && (modenum < 1)) ||
	   (!windowed && (modenum >= nummodes)))
	{
		Sys_Error("Bad video mode\n");
	}

	// so Con_Printfs don't mess us up by forcing vid and snd updates
	temp = scr_disabled_for_loading;
	scr_disabled_for_loading = true;

	CDAudio_Pause();

	if(vid_modenum == NO_MODE)
		original_mode = windowed_default;
	else
		original_mode = vid_modenum;

	// Set either the fullscreen or windowed mode
	if(modelist[modenum].type == MS_WINDOWED)
	{
		if(_windowed_mouse.value && key_dest == key_game)
		{
			stat = VID_SetWindowedMode(modenum);
			IN_ActivateMouse();
			IN_HideMouse();
		}
		else
		{
			IN_DeactivateMouse();
			IN_ShowMouse();
			stat = VID_SetWindowedMode(modenum);
		}
	}
	else if(modelist[modenum].type == MS_FULLDIB)
	{
		stat = VID_SetFullDIBMode(modenum);
		IN_ActivateMouse();
		IN_HideMouse();
	}
	else
	{
		Sys_Error("VID_SetMode: Bad mode type in modelist");
	}

	window_width = DIBWidth;
	window_height = DIBHeight;
	VID_UpdateWindowStatus();

	CDAudio_Resume();
	scr_disabled_for_loading = temp;

	if(!stat)
	{
		Sys_Error("Couldn't set video mode");
	}

	// now we try to make sure we get the focus on the mode switch, because
	// sometimes in some systems we don't.  We grab the foreground, then
	// finish setting up, pump all our messages, and sleep for a little while
	// to let messages finish bouncing around the system, then we put
	// ourselves at the top of the z order, then grab the foreground again,
	// Who knows if it helps, but it probably doesn't hurt
	SetForegroundWindow(mainwindow);
	VID_SetPalette(palette);
	vid_modenum = modenum;
	Cvar_SetValue("vid_mode", (float)vid_modenum);

	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(100);

	SetWindowPos(mainwindow, HWND_TOP, 0, 0, 0, 0,
	             SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW |
	             SWP_NOCOPYBITS);

	SetForegroundWindow(mainwindow);

	// fix the leftover Alt from any Alt-Tab or the like that switched us away
	ClearAllStates();

	if(!msg_suppress_1)
		Con_SafePrintf("Video mode %s initialized.\n", VID_GetModeDescription(vid_modenum));

	VID_SetPalette(palette);

	vid.recalc_refdef = 1;

	return true;
}

/*
================
VID_UpdateWindowStatus
================
*/
void VID_UpdateWindowStatus()
{
	window_rect.left = window_x;
	window_rect.top = window_y;
	window_rect.right = window_x + window_width;
	window_rect.bottom = window_y + window_height;
	window_center_x = (window_rect.left + window_rect.right) / 2;
	window_center_y = (window_rect.top + window_rect.bottom) / 2;

	IN_UpdateClipCursor();
}

//====================================


//int		texture_mode = GL_NEAREST;
//int		texture_mode = GL_NEAREST_MIPMAP_NEAREST;
//int		texture_mode = GL_NEAREST_MIPMAP_LINEAR;
int texture_mode = GL_LINEAR;
//int		texture_mode = GL_LINEAR_MIPMAP_NEAREST;
//int		texture_mode = GL_LINEAR_MIPMAP_LINEAR;

int texture_extension_number = 1;

/*
=================
GL_BeginRendering

=================
*/
void GL_BeginRendering(int *x, int *y, int *width, int *height)
{
	extern cvar_t gl_clear;

	*x = *y = 0;
	*width = WindowRect.right - WindowRect.left;
	*height = WindowRect.bottom - WindowRect.top;

	//    if (!wglMakeCurrent( maindc, baseRC ))
	//		Sys_Error ("wglMakeCurrent failed");

	//	glViewport (*x, *y, *width, *height);
}

void GL_EndRendering()
{
	if(!scr_skipupdate || block_drawing)
		SwapBuffers(maindc);

	// handle the mouse state when windowed if that's changed
	if(modestate == MS_WINDOWED)
	{
		if(!_windowed_mouse.value)
		{
			if(windowed_mouse)
			{
				IN_DeactivateMouse();
				IN_ShowMouse();
				windowed_mouse = false;
			}
		}
		else
		{
			windowed_mouse = true;
			if(key_dest == key_game && !mouseactive && ActiveApp)
			{
				IN_ActivateMouse();
				IN_HideMouse();
			}
			else if(mouseactive && key_dest != key_game)
			{
				IN_DeactivateMouse();
				IN_ShowMouse();
			}
		}
	}
	if(fullsbardraw)
		Sbar_Changed();
}

void VID_SetPalette(unsigned char *palette)
{
	byte *pal;
	unsigned r, g, b;
	unsigned v;
	int r1, g1, b1;
	int j, k, l, m;
	unsigned short i;
	unsigned *table;
	FILE *f;
	char s[255];
	HWND hDlg, hProgress;
	float gamma;

	//
	// 8 8 8 encoding
	//
	pal = palette;
	table = d_8to24table;
	for(i = 0; i < 256; i++)
	{
		r = pal[0];
		g = pal[1];
		b = pal[2];
		pal += 3;

		//		v = (255<<24) + (r<<16) + (g<<8) + (b<<0);
		//		v = (255<<0) + (r<<8) + (g<<16) + (b<<24);
		v = (255 << 24) + (r << 0) + (g << 8) + (b << 16);
		*table++ = v;
	}
	d_8to24table[255] &= 0xffffff; // 255 is transparent

	// JACK: 3D distance calcs - k is last closest, l is the distance.
	// FIXME: Precalculate this and cache to disk.
	for(i = 0; i < (1 << 15); i++)
	{
		/* Maps
			000000000000000
			000000000011111 = Red  = 0x1F
			000001111100000 = Blue = 0x03E0
			111110000000000 = Grn  = 0x7C00
		*/
		r = ((i & 0x1F) << 3) + 4;
		g = ((i & 0x03E0) >> 2) + 4;
		b = ((i & 0x7C00) >> 7) + 4;
		pal = (unsigned char *)d_8to24table;
		for(v = 0, k = 0, l = 10000 * 10000; v < 256; v++, pal += 4)
		{
			r1 = r - pal[0];
			g1 = g - pal[1];
			b1 = b - pal[2];
			j = (r1 * r1) + (g1 * g1) + (b1 * b1);
			if(j < l)
			{
				k = v;
				l = j;
			}
		}
		d_15to8table[i] = k;
	}
}

BOOL gammaworks;

void VID_ShiftPalette(unsigned char *palette)
{
	extern byte ramps[3][256];

	//	VID_SetPalette (palette);

	//	gammaworks = SetDeviceGammaRamp (maindc, ramps);
}

void VID_SetDefaultMode()
{
	IN_DeactivateMouse();
}

//==========================================================================

BOOL bSetupPixelFormat(HDC hDC)
{
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR), // size of this pfd
		1,                             // version number
		PFD_DRAW_TO_WINDOW             // support window
		| PFD_SUPPORT_OPENGL           // support OpenGL
		| PFD_DOUBLEBUFFER,            // double buffered
		PFD_TYPE_RGBA,                 // RGBA type
		24,                            // 24-bit color depth
		0,
		0, 0, 0, 0, 0,  // color bits ignored
		0,              // no alpha buffer
		0,              // shift bit ignored
		0,              // no accumulation buffer
		0, 0, 0, 0,     // accum bits ignored
		32,             // 32-bit z-buffer
		0,              // no stencil buffer
		0,              // no auxiliary buffer
		PFD_MAIN_PLANE, // main layer
		0,              // reserved
		0, 0, 0         // layer masks ignored
	};
	int pixelformat;

	if((pixelformat = ChoosePixelFormat(hDC, &pfd)) == 0)
	{
		MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
		return FALSE;
	}

	if(SetPixelFormat(hDC, pixelformat, &pfd) == FALSE)
	{
		MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
		return FALSE;
	}

	return TRUE;
}

byte scantokey[128] =
{
  //  0           1       2       3       4       5       6       7
  //  8           9       A       B       C       D       E       F
  0, 27, '1', '2', '3', '4', '5', '6',
  '7', '8', '9', '0', '-', '=', K_BACKSPACE, 9, // 0
  'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
  'o', 'p', '[', ']', 13, K_CTRL, 'a', 's', // 1
  'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
  '\'', '`', K_SHIFT, '\\', 'z', 'x', 'c', 'v', // 2
  'b', 'n', 'm', ',', '.', '/', K_SHIFT, '*',
  K_ALT, ' ', 0, K_F1, K_F2, K_F3, K_F4, K_F5, // 3
  K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, 0, K_HOME,
  K_UPARROW, K_PGUP, '-', K_LEFTARROW, '5', K_RIGHTARROW, '+', K_END, //4
  K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, 0, K_F11,
  K_F12, 0, 0, 0, 0, 0, 0, 0, // 5
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, // 6
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0 // 7
};

byte shiftscantokey[128] =
{
  //  0           1       2       3       4       5       6       7
  //  8           9       A       B       C       D       E       F
  0, 27, '!', '@', '#', '$', '%', '^',
  '&', '*', '(', ')', '_', '+', K_BACKSPACE, 9, // 0
  'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
  'O', 'P', '{', '}', 13, K_CTRL, 'A', 'S', // 1
  'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
  '"', '~', K_SHIFT, '|', 'Z', 'X', 'C', 'V', // 2
  'B', 'N', 'M', '<', '>', '?', K_SHIFT, '*',
  K_ALT, ' ', 0, K_F1, K_F2, K_F3, K_F4, K_F5, // 3
  K_F6, K_F7, K_F8, K_F9, K_F10, K_PAUSE, 0, K_HOME,
  K_UPARROW, K_PGUP, '_', K_LEFTARROW, '%', K_RIGHTARROW, '+', K_END, //4
  K_DOWNARROW, K_PGDN, K_INS, K_DEL, 0, 0, 0, K_F11,
  K_F12, 0, 0, 0, 0, 0, 0, 0, // 5
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, // 6
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0 // 7
};

/*
===================================================================

MAIN WINDOW

===================================================================
*/

/*
================
ClearAllStates
================
*/
void ClearAllStates()
{
	int i;

	// send an up event for each key, to make sure the server clears them all
	for(i = 0; i < 256; i++)
	{
		Key_Event(i, false);
	}

	Key_ClearStates();
	IN_ClearStates();
}

void AppActivate(BOOL fActive, BOOL minimize)
/****************************************************************************
*
* Function:     AppActivate
* Parameters:   fActive - True if app is activating
*
* Description:  If the application is activating, then swap the system
*               into SYSPAL_NOSTATIC mode so that our palettes will display
*               correctly.
*
****************************************************************************/
{
	MSG msg;
	HDC hdc;
	int i, t;
	static BOOL sound_active;

	ActiveApp = fActive;
	Minimized = minimize;

	// enable/disable sound on focus gain/loss
	if(!ActiveApp && sound_active)
	{
		S_BlockSound();
		sound_active = false;
	}
	else if(ActiveApp && !sound_active)
	{
		S_UnblockSound();
		sound_active = true;
	}

	if(fActive)
	{
		if(modestate == MS_FULLDIB)
		{
			IN_ActivateMouse();
			IN_HideMouse();
			if(vid_canalttab && vid_wassuspended)
			{
				vid_wassuspended = false;
				ChangeDisplaySettings(&gdevmode, CDS_FULLSCREEN);
				ShowWindow(mainwindow, SW_SHOWNORMAL);
			}
		}
		else if((modestate == MS_WINDOWED) && _windowed_mouse.value && key_dest == key_game)
		{
			IN_ActivateMouse();
			IN_HideMouse();
		}
	}

	if(!fActive)
	{
		if(modestate == MS_FULLDIB)
		{
			IN_DeactivateMouse();
			IN_ShowMouse();
			if(vid_canalttab)
			{
				ChangeDisplaySettings(NULL, 0);
				vid_wassuspended = true;
			}
		}
		else if((modestate == MS_WINDOWED) && _windowed_mouse.value)
		{
			IN_DeactivateMouse();
			IN_ShowMouse();
		}
	}
}

/*
=================
VID_NumModes
=================
*/
int VID_NumModes()
{
	return nummodes;
}

/*
=================
VID_GetModePtr
=================
*/
vmode_t *VID_GetModePtr(int modenum)
{
	if((modenum >= 0) && (modenum < nummodes))
		return &modelist[modenum];
	else
		return &badmode;
}

/*
=================
VID_GetModeDescription
=================
*/
char *VID_GetModeDescription(int mode)
{
	char *pinfo;
	vmode_t *pv;
	static char temp[100];

	if((mode < 0) || (mode >= nummodes))
		return NULL;

	if(!leavecurrentmode)
	{
		pv = VID_GetModePtr(mode);
		pinfo = pv->modedesc;
	}
	else
	{
		sprintf(temp, "Desktop resolution (%dx%d)",
		        modelist[MODE_FULLSCREEN_DEFAULT].width,
		        modelist[MODE_FULLSCREEN_DEFAULT].height);
		pinfo = temp;
	}

	return pinfo;
}

// KJB: Added this to return the mode driver name in description for console

char *VID_GetExtModeDescription(int mode)
{
	static char pinfo[40];
	vmode_t *pv;

	if((mode < 0) || (mode >= nummodes))
		return NULL;

	pv = VID_GetModePtr(mode);
	if(modelist[mode].type == MS_FULLDIB)
	{
		if(!leavecurrentmode)
		{
			sprintf(pinfo, "%s fullscreen", pv->modedesc);
		}
		else
		{
			sprintf(pinfo, "Desktop resolution (%dx%d)",
			        modelist[MODE_FULLSCREEN_DEFAULT].width,
			        modelist[MODE_FULLSCREEN_DEFAULT].height);
		}
	}
	else
	{
		if(modestate == MS_WINDOWED)
			sprintf(pinfo, "%s windowed", pv->modedesc);
		else
			sprintf(pinfo, "windowed");
	}

	return pinfo;
}

/*
=================
VID_DescribeCurrentMode_f
=================
*/
void VID_DescribeCurrentMode_f()
{
	Con_Printf("%s\n", VID_GetExtModeDescription(vid_modenum));
}

/*
=================
VID_NumModes_f
=================
*/
void VID_NumModes_f()
{
	if(nummodes == 1)
		Con_Printf("%d video mode is available\n", nummodes);
	else
		Con_Printf("%d video modes are available\n", nummodes);
}

/*
=================
VID_DescribeMode_f
=================
*/
void VID_DescribeMode_f()
{
	int t, modenum;

	modenum = Q_atoi(Cmd_Argv(1));

	t = leavecurrentmode;
	leavecurrentmode = 0;

	Con_Printf("%s\n", VID_GetExtModeDescription(modenum));

	leavecurrentmode = t;
}

/*
=================
VID_DescribeModes_f
=================
*/
void VID_DescribeModes_f()
{
	int i, lnummodes, t;
	char *pinfo;
	vmode_t *pv;

	lnummodes = VID_NumModes();

	t = leavecurrentmode;
	leavecurrentmode = 0;

	for(i = 1; i < lnummodes; i++)
	{
		pv = VID_GetModePtr(i);
		pinfo = VID_GetExtModeDescription(i);
		Con_Printf("%2d: %s\n", i, pinfo);
	}

	leavecurrentmode = t;
}

void VID_InitDIB(HINSTANCE hInstance)
{
	WNDCLASS wc;
	HDC hdc;
	int i;

	/* Register the frame class */
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = 0;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = 0;
	wc.lpszClassName = "WinQuake";

	if(!RegisterClass(&wc))
		Sys_Error("Couldn't register window class");

	modelist[0].type = MS_WINDOWED;

	if(COM_CheckParm("-width"))
		modelist[0].width = Q_atoi(com_argv[COM_CheckParm("-width") + 1]);
	else
		modelist[0].width = 640;

	if(modelist[0].width < 320)
		modelist[0].width = 320;

	if(COM_CheckParm("-height"))
		modelist[0].height = Q_atoi(com_argv[COM_CheckParm("-height") + 1]);
	else
		modelist[0].height = modelist[0].width * 240 / 320;

	if(modelist[0].height < 240)
		modelist[0].height = 240;

	sprintf(modelist[0].modedesc, "%dx%d",
	        modelist[0].width, modelist[0].height);

	modelist[0].modenum = MODE_WINDOWED;
	modelist[0].dib = 1;
	modelist[0].fullscreen = 0;
	modelist[0].halfscreen = 0;
	modelist[0].bpp = 0;

	nummodes = 1;
}

/*
=================
VID_InitFullDIB
=================
*/
void VID_InitFullDIB(HINSTANCE hInstance)
{
	DEVMODE devmode;
	int i, modenum, cmodes, originalnummodes, existingmode, numlowresmodes;
	int j, bpp, done;
	BOOL stat;

	// enumerate >8 bpp modes
	originalnummodes = nummodes;
	modenum = 0;

	do
	{
		stat = EnumDisplaySettings(NULL, modenum, &devmode);

		if((devmode.dmBitsPerPel >= 15) &&
		   (devmode.dmPelsWidth <= MAXWIDTH) &&
		   (devmode.dmPelsHeight <= MAXHEIGHT) &&
		   (nummodes < MAX_MODE_LIST))
		{
			devmode.dmFields = DM_BITSPERPEL |
			DM_PELSWIDTH |
			DM_PELSHEIGHT;

			if(ChangeDisplaySettings(&devmode, CDS_TEST | CDS_FULLSCREEN) ==
			   DISP_CHANGE_SUCCESSFUL)
			{
				modelist[nummodes].type = MS_FULLDIB;
				modelist[nummodes].width = devmode.dmPelsWidth;
				modelist[nummodes].height = devmode.dmPelsHeight;
				modelist[nummodes].modenum = 0;
				modelist[nummodes].halfscreen = 0;
				modelist[nummodes].dib = 1;
				modelist[nummodes].fullscreen = 1;
				modelist[nummodes].bpp = devmode.dmBitsPerPel;
				sprintf(modelist[nummodes].modedesc, "%dx%dx%d",
				        devmode.dmPelsWidth, devmode.dmPelsHeight,
				        devmode.dmBitsPerPel);

				// if the width is more than twice the height, reduce it by half because this
				// is probably a dual-screen monitor
				if(!COM_CheckParm("-noadjustaspect"))
				{
					if(modelist[nummodes].width > (modelist[nummodes].height << 1))
					{
						modelist[nummodes].width >>= 1;
						modelist[nummodes].halfscreen = 1;
						sprintf(modelist[nummodes].modedesc, "%dx%dx%d",
						        modelist[nummodes].width,
						        modelist[nummodes].height,
						        modelist[nummodes].bpp);
					}
				}

				for(i = originalnummodes, existingmode = 0; i < nummodes; i++)
				{
					if((modelist[nummodes].width == modelist[i].width) &&
					   (modelist[nummodes].height == modelist[i].height) &&
					   (modelist[nummodes].bpp == modelist[i].bpp))
					{
						existingmode = 1;
						break;
					}
				}

				if(!existingmode)
				{
					nummodes++;
				}
			}
		}

		modenum++;
	} while(stat);

	// see if there are any low-res modes that aren't being reported
	numlowresmodes = sizeof(lowresmodes) / sizeof(lowresmodes[0]);
	bpp = 16;
	done = 0;

	do
	{
		for(j = 0; (j < numlowresmodes) && (nummodes < MAX_MODE_LIST); j++)
		{
			devmode.dmBitsPerPel = bpp;
			devmode.dmPelsWidth = lowresmodes[j].width;
			devmode.dmPelsHeight = lowresmodes[j].height;
			devmode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

			if(ChangeDisplaySettings(&devmode, CDS_TEST | CDS_FULLSCREEN) ==
			   DISP_CHANGE_SUCCESSFUL)
			{
				modelist[nummodes].type = MS_FULLDIB;
				modelist[nummodes].width = devmode.dmPelsWidth;
				modelist[nummodes].height = devmode.dmPelsHeight;
				modelist[nummodes].modenum = 0;
				modelist[nummodes].halfscreen = 0;
				modelist[nummodes].dib = 1;
				modelist[nummodes].fullscreen = 1;
				modelist[nummodes].bpp = devmode.dmBitsPerPel;
				sprintf(modelist[nummodes].modedesc, "%dx%dx%d",
				        devmode.dmPelsWidth, devmode.dmPelsHeight,
				        devmode.dmBitsPerPel);

				for(i = originalnummodes, existingmode = 0; i < nummodes; i++)
				{
					if((modelist[nummodes].width == modelist[i].width) &&
					   (modelist[nummodes].height == modelist[i].height) &&
					   (modelist[nummodes].bpp == modelist[i].bpp))
					{
						existingmode = 1;
						break;
					}
				}

				if(!existingmode)
				{
					nummodes++;
				}
			}
		}
		switch(bpp)
		{
		case 16:
			bpp = 32;
			break;

		case 32:
			bpp = 24;
			break;

		case 24:
			done = 1;
			break;
		}
	} while(!done);

	if(nummodes == originalnummodes)
		Con_SafePrintf("No fullscreen DIB modes found\n");
}

qboolean VID_Is8bit()
{
	return is8bit;
}

#define GL_SHARED_TEXTURE_PALETTE_EXT 0x81FB

void VID_Init8bitPalette()
{
	// Check for 8bit Extensions and initialize them.
	int i;
	char thePalette[256 * 3];
	char *oldPalette, *newPalette;

	glColorTableEXT = (void *)wglGetProcAddress("glColorTableEXT");
	if(!glColorTableEXT || strstr(gl_extensions, "GL_EXT_shared_texture_palette") ||
	   COM_CheckParm("-no8bit"))
		return;

	Con_SafePrintf("8-bit GL extensions enabled.\n");
	glEnable(GL_SHARED_TEXTURE_PALETTE_EXT);
	oldPalette = (char *)d_8to24table; //d_8to24table3dfx;
	newPalette = thePalette;
	for(i = 0; i < 256; i++)
	{
		*newPalette++ = *oldPalette++;
		*newPalette++ = *oldPalette++;
		*newPalette++ = *oldPalette++;
		oldPalette++;
	}
	glColorTableEXT(GL_SHARED_TEXTURE_PALETTE_EXT, GL_RGB, 256, GL_RGB, GL_UNSIGNED_BYTE,
	                (void *)thePalette);
	is8bit = TRUE;
}

static void Check_Gamma(unsigned char *pal)
{
	float f, inf;
	unsigned char palette[768];
	int i;

	if((i = COM_CheckParm("-gamma")) == 0)
	{
		if((gl_renderer && strstr(gl_renderer, "Voodoo")) ||
		   (gl_vendor && strstr(gl_vendor, "3Dfx")))
			vid_gamma = 1;
		else
			vid_gamma = 0.7; // default to 0.7 on non-3dfx hardware
	}
	else
		vid_gamma = Q_atof(com_argv[i + 1]);

	for(i = 0; i < 768; i++)
	{
		f = pow((pal[i] + 1) / 256.0, vid_gamma);
		inf = f * 255 + 0.5;
		if(inf < 0)
			inf = 0;
		if(inf > 255)
			inf = 255;
		palette[i] = inf;
	}

	memcpy(pal, palette, sizeof(palette));
}

//========================================================
// Video menu stuff
//========================================================

extern void M_Menu_Options_f();
extern void M_Print(int cx, int cy, char *str);
extern void M_PrintWhite(int cx, int cy, char *str);
extern void M_DrawCharacter(int cx, int line, int num);
extern void M_DrawTransPic(int x, int y, qpic_t *pic);
extern void M_DrawPic(int x, int y, qpic_t *pic);

static int vid_line, vid_wmodes;

typedef struct
{
	int modenum;
	char *desc;
	int iscur;
} modedesc_t;

#define MAX_COLUMN_SIZE 9
#define MODE_AREA_HEIGHT (MAX_COLUMN_SIZE + 2)
#define MAX_MODEDESCS (MAX_COLUMN_SIZE * 3)

static modedesc_t modedescs[MAX_MODEDESCS];

/*
================
VID_MenuDraw
================
*/
void VID_MenuDraw()
{
	qpic_t *p;
	char *ptr;
	int lnummodes, i, j, k, column, row, dup, dupmode;
	char temp[100];
	vmode_t *pv;

	p = Draw_CachePic("gfx/vidmodes.lmp");
	M_DrawPic((320 - p->width) / 2, 4, p);

	vid_wmodes = 0;
	lnummodes = VID_NumModes();

	for(i = 1; (i < lnummodes) && (vid_wmodes < MAX_MODEDESCS); i++)
	{
		ptr = VID_GetModeDescription(i);
		pv = VID_GetModePtr(i);

		k = vid_wmodes;

		modedescs[k].modenum = i;
		modedescs[k].desc = ptr;
		modedescs[k].iscur = 0;

		if(i == vid_modenum)
			modedescs[k].iscur = 1;

		vid_wmodes++;
	}

	if(vid_wmodes > 0)
	{
		M_Print(2 * 8, 36 + 0 * 8, "Fullscreen Modes (WIDTHxHEIGHTxBPP)");

		column = 8;
		row = 36 + 2 * 8;

		for(i = 0; i < vid_wmodes; i++)
		{
			if(modedescs[i].iscur)
				M_PrintWhite(column, row, modedescs[i].desc);
			else
				M_Print(column, row, modedescs[i].desc);

			column += 13 * 8;

			if((i % VID_ROW_SIZE) == (VID_ROW_SIZE - 1))
			{
				column = 8;
				row += 8;
			}
		}
	}

	M_Print(3 * 8, 36 + MODE_AREA_HEIGHT * 8 + 8 * 2,
	        "Video modes must be set from the");
	M_Print(3 * 8, 36 + MODE_AREA_HEIGHT * 8 + 8 * 3,
	        "command line with -width <width>");
	M_Print(3 * 8, 36 + MODE_AREA_HEIGHT * 8 + 8 * 4,
	        "and -bpp <bits-per-pixel>");
	M_Print(3 * 8, 36 + MODE_AREA_HEIGHT * 8 + 8 * 6,
	        "Select windowed mode with -window");
}

/*
================
VID_MenuKey
================
*/
void VID_MenuKey(int key)
{
	switch(key)
	{
	case K_ESCAPE:
		S_LocalSound("misc/menu1.wav");
		M_Menu_Options_f();
		break;

	default:
		break;
	}
}
