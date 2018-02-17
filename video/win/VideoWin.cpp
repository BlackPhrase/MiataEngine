#include "VideoWin.hpp"

LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//====================================

/*
=======
MapKey

Map from windows to quake keynums
=======
*/
int MapKey(int key)
{
	key = (key >> 16) & 255;
	if(key > 127)
		return 0;
	if(scantokey[key] == 0)
		Con_DPrintf("key 0x%02x has no translation\n", key);
	return scantokey[key];
}

/* main window procedure */
LONG WINAPI MainWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG lRet = 1;
	int fwKeys, xPos, yPos, fActive, fMinimized, temp;
	extern unsigned int uiWheelMessage;

	if(uMsg == uiWheelMessage)
		uMsg = WM_MOUSEWHEEL;

	switch(uMsg)
	{
	case WM_KILLFOCUS:
		if(modestate == MS_FULLDIB)
			ShowWindow(mainwindow, SW_SHOWMINNOACTIVE);
		break;

	case WM_CREATE:
		break;

	case WM_MOVE:
		window_x = (int)LOWORD(lParam);
		window_y = (int)HIWORD(lParam);
		VID_UpdateWindowStatus();
		break;

	case WM_SYSCHAR:
		// keep Alt-Space from happening
		break;

	case WM_SIZE:
		break;

	case WM_CLOSE:
		if(MessageBox(mainwindow, "Are you sure you want to quit?", "Confirm Exit",
		              MB_YESNO | MB_SETFOREGROUND | MB_ICONQUESTION) == IDYES)
		{
			Sys_Quit();
		}

		break;

	case WM_ACTIVATE:
		fActive = LOWORD(wParam);
		fMinimized = (BOOL)HIWORD(wParam);
		AppActivate(!(fActive == WA_INACTIVE), fMinimized);

		// fix the leftover Alt from any Alt-Tab or the like that switched us away
		ClearAllStates();

		break;

	case WM_DESTROY:
	{
		if(dibwindow)
			DestroyWindow(dibwindow);

		PostQuitMessage(0);
	}
	break;

	case MM_MCINOTIFY:
		lRet = CDAudio_MessageHandler(hWnd, uMsg, wParam, lParam);
		break;

	default:
		/* pass all unhandled messages to DefWindowProc */
		lRet = DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}

	/* return 1 if handled message, 0 if not */
	return lRet;
};

/*
===================
VID_Init
===================
*/
void CVideoWin::Init(unsigned char *palette)
{
	int i, existingmode;
	int basenummodes, width, height, bpp, findbpp, done;
	byte *ptmp;
	char gldir[MAX_OSPATH];
	HDC hdc;
	DEVMODE devmode;

	memset(&devmode, 0, sizeof(devmode));

	Cvar_RegisterVariable(&vid_mode);
	Cvar_RegisterVariable(&vid_wait);
	Cvar_RegisterVariable(&vid_nopageflip);
	Cvar_RegisterVariable(&_vid_wait_override);
	Cvar_RegisterVariable(&_vid_default_mode);
	Cvar_RegisterVariable(&_vid_default_mode_win);
	Cvar_RegisterVariable(&vid_config_x);
	Cvar_RegisterVariable(&vid_config_y);
	Cvar_RegisterVariable(&vid_stretch_by_2);
	Cvar_RegisterVariable(&_windowed_mouse);
	Cvar_RegisterVariable(&gl_ztrick);

	Cmd_AddCommand("vid_nummodes", VID_NumModes_f);
	Cmd_AddCommand("vid_describecurrentmode", VID_DescribeCurrentMode_f);
	Cmd_AddCommand("vid_describemode", VID_DescribeMode_f);
	Cmd_AddCommand("vid_describemodes", VID_DescribeModes_f);

	hIcon = LoadIcon(global_hInstance, MAKEINTRESOURCE(IDI_ICON2));

	InitCommonControls();

	VID_InitDIB(global_hInstance);
	basenummodes = nummodes = 1;

	VID_InitFullDIB(global_hInstance);

	if(COM_CheckParm("-window"))
	{
		hdc = GetDC(NULL);

		if(GetDeviceCaps(hdc, RASTERCAPS) & RC_PALETTE)
		{
			Sys_Error("Can't run in non-RGB mode");
		}

		ReleaseDC(NULL, hdc);

		windowed = true;

		vid_default = MODE_WINDOWED;
	}
	else
	{
		if(nummodes == 1)
			Sys_Error("No RGB fullscreen modes available");

		windowed = false;

		if(COM_CheckParm("-mode"))
		{
			vid_default = Q_atoi(com_argv[COM_CheckParm("-mode") + 1]);
		}
		else
		{
			if(COM_CheckParm("-current"))
			{
				modelist[MODE_FULLSCREEN_DEFAULT].width =
				GetSystemMetrics(SM_CXSCREEN);
				modelist[MODE_FULLSCREEN_DEFAULT].height =
				GetSystemMetrics(SM_CYSCREEN);
				vid_default = MODE_FULLSCREEN_DEFAULT;
				leavecurrentmode = 1;
			}
			else
			{
				if(COM_CheckParm("-width"))
				{
					width = Q_atoi(com_argv[COM_CheckParm("-width") + 1]);
				}
				else
				{
					width = 640;
				}

				if(COM_CheckParm("-bpp"))
				{
					bpp = Q_atoi(com_argv[COM_CheckParm("-bpp") + 1]);
					findbpp = 0;
				}
				else
				{
					bpp = 15;
					findbpp = 1;
				}

				if(COM_CheckParm("-height"))
					height = Q_atoi(com_argv[COM_CheckParm("-height") + 1]);

				// if they want to force it, add the specified mode to the list
				if(COM_CheckParm("-force") && (nummodes < MAX_MODE_LIST))
				{
					modelist[nummodes].type = MS_FULLDIB;
					modelist[nummodes].width = width;
					modelist[nummodes].height = height;
					modelist[nummodes].modenum = 0;
					modelist[nummodes].halfscreen = 0;
					modelist[nummodes].dib = 1;
					modelist[nummodes].fullscreen = 1;
					modelist[nummodes].bpp = bpp;
					sprintf(modelist[nummodes].modedesc, "%dx%dx%d",
					        devmode.dmPelsWidth, devmode.dmPelsHeight,
					        devmode.dmBitsPerPel);

					for(i = nummodes, existingmode = 0; i < nummodes; i++)
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

				done = 0;

				do
				{
					if(COM_CheckParm("-height"))
					{
						height = Q_atoi(com_argv[COM_CheckParm("-height") + 1]);

						for(i = 1, vid_default = 0; i < nummodes; i++)
						{
							if((modelist[i].width == width) &&
							   (modelist[i].height == height) &&
							   (modelist[i].bpp == bpp))
							{
								vid_default = i;
								done = 1;
								break;
							}
						}
					}
					else
					{
						for(i = 1, vid_default = 0; i < nummodes; i++)
						{
							if((modelist[i].width == width) && (modelist[i].bpp == bpp))
							{
								vid_default = i;
								done = 1;
								break;
							}
						}
					}

					if(!done)
					{
						if(findbpp)
						{
							switch(bpp)
							{
							case 15:
								bpp = 16;
								break;
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
						}
						else
						{
							done = 1;
						}
					}
				} while(!done);

				if(!vid_default)
				{
					Sys_Error("Specified video mode not available");
				}
			}
		}
	}

	vid_initialized = true;

	if((i = COM_CheckParm("-conwidth")) != 0)
		vid.conwidth = Q_atoi(com_argv[i + 1]);
	else
		vid.conwidth = 640;

	vid.conwidth &= 0xfff8; // make it a multiple of eight

	if(vid.conwidth < 320)
		vid.conwidth = 320;

	// pick a conheight that matches with correct aspect
	vid.conheight = vid.conwidth * 3 / 4;

	if((i = COM_CheckParm("-conheight")) != 0)
		vid.conheight = Q_atoi(com_argv[i + 1]);
	if(vid.conheight < 200)
		vid.conheight = 200;

	vid.maxwarpwidth = WARP_WIDTH;
	vid.maxwarpheight = WARP_HEIGHT;
	vid.colormap = host_colormap;
	vid.fullbright = 256 - LittleLong(*((int *)vid.colormap + 2048));

	DestroyWindow(hwnd_dialog);

	Check_Gamma(palette);
	VID_SetPalette(palette);

	VID_SetMode(vid_default, palette);

	maindc = GetDC(mainwindow);
	bSetupPixelFormat(maindc);

	baseRC = wglCreateContext(maindc);
	if(!baseRC)
		Sys_Error("Could not initialize GL (wglCreateContext failed).\n\nMake sure you in are 65535 color mode, and try running -window.");
	if(!wglMakeCurrent(maindc, baseRC))
		Sys_Error("wglMakeCurrent failed");

	GL_Init();

	sprintf(gldir, "%s/glquake", com_gamedir);
	Sys_mkdir(gldir);

	vid_realmode = vid_modenum;

	// Check for 3DFX Extensions and initialize them.
	VID_Init8bitPalette();

	vid_menudrawfn = VID_MenuDraw;
	vid_menukeyfn = VID_MenuKey;

	strcpy(badmode.modedesc, "Bad mode");
	vid_canalttab = true;

	if(COM_CheckParm("-fullsbar"))
		fullsbardraw = true;
};

void CVideoWin::Shutdown()
{
	HGLRC hRC;
	HDC hDC;

	if(vid_initialized)
	{
		vid_canalttab = false;
		hRC = wglGetCurrentContext();
		hDC = wglGetCurrentDC();

		wglMakeCurrent(NULL, NULL);

		if(hRC)
			wglDeleteContext(hRC);

		if(hDC && dibwindow)
			ReleaseDC(dibwindow, hDC);

		if(modestate == MS_FULLDIB)
			ChangeDisplaySettings(NULL, 0);

		if(maindc && dibwindow)
			ReleaseDC(dibwindow, maindc);

		AppActivate(false, false);
	};
};