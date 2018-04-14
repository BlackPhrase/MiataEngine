#include "MouseWin.hpp"

#define DINPUT_BUFFERSIZE 16
#define iDirectInputCreate(a, b, c, d) pDirectInputCreate(a, b, c, d)

HRESULT(WINAPI *pDirectInputCreate)(HINSTANCE hinst, DWORD dwVersion, LPDIRECTINPUT *lplpDirectInput, LPUNKNOWN punkOuter);

int mouse_buttons;
int mouse_oldbuttonstate;

POINT current_pos;

int mouse_x, mouse_y, old_mouse_x, old_mouse_y, mx_accum, my_accum;

static int originalmouseparms[3], newmouseparms[3] = { 0, 0, 1 };

static bool mouseparmsvalid, mouseactivatetoggle;
static bool mouseshowtoggle = true;

unsigned int uiWheelMessage;

// mouse variables
cvar_t m_filter = { "m_filter", "0" };

void CMouseWin::Init()
{
	// mouse variables
	Cvar_RegisterVariable(&m_filter);
	
	uiWheelMessage = RegisterWindowMessage("MSWHEEL_ROLLMSG");
};

void CMouseWin::Shutdown()
{
	if(g_pMouse)
	{
		IDirectInputDevice_Release(g_pMouse);
		g_pMouse = nullptr;
	};

	if(g_pdi)
	{
		IDirectInput_Release(g_pdi);
		g_pdi = nullptr;
	};
};

/*
===========
IN_StartupMouse
===========
*/
void CMouseWin::Startup()
{
	HDC hdc;

	if(COM_CheckParm("-nomouse"))
		return;

	mouseinitialized = true;

	if(COM_CheckParm("-dinput"))
	{
		dinput = InitDInput();

		if(dinput)
		{
			Con_SafePrintf("DirectInput initialized\n");
		}
		else
		{
			Con_SafePrintf("DirectInput not initialized\n");
		};
	};

	if(!dinput)
	{
		mouseparmsvalid = SystemParametersInfo(SPI_GETMOUSE, 0, originalmouseparms, 0);

		if(mouseparmsvalid)
		{
			if(COM_CheckParm("-noforcemspd"))
				newmouseparms[2] = originalmouseparms[2];

			if(COM_CheckParm("-noforcemaccel"))
			{
				newmouseparms[0] = originalmouseparms[0];
				newmouseparms[1] = originalmouseparms[1];
			};

			if(COM_CheckParm("-noforcemparms"))
			{
				newmouseparms[0] = originalmouseparms[0];
				newmouseparms[1] = originalmouseparms[1];
				newmouseparms[2] = originalmouseparms[2];
			};
		};
	};

	mouse_buttons = 3;

	// if a fullscreen video mode was set before the mouse was initialized,
	// set the mouse state appropriately
	if(mouseactivatetoggle)
		Activate();
};

/*
===================
IN_ClearStates
===================
*/
void CMouseWin::ClearStates()
{
	if(mouseactive)
	{
		mx_accum = 0;
		my_accum = 0;
		mouse_oldbuttonstate = 0;
	};
};

/*
===========
IN_ActivateMouse
===========
*/
void CMouseWin::Activate()
{
	mouseactivatetoggle = true;

	if(mouseinitialized)
	{
		if(dinput)
		{
			if(g_pMouse)
			{
				if(!dinput_acquired)
				{
					IDirectInputDevice_Acquire(g_pMouse);
					dinput_acquired = true;
				};
			}
			else
			{
				return;
			};
		}
		else
		{
			if(mouseparmsvalid)
				restore_spi = SystemParametersInfo(SPI_SETMOUSE, 0, newmouseparms, 0);

			SetCursorPos(window_center_x, window_center_y);
			SetCapture(mainwindow);
			ClipCursor(&window_rect);
		};

		mouseactive = true;
	};
};

/*
===========
IN_DeactivateMouse
===========
*/
void CMouseWin::Deactivate()
{
	mouseactivatetoggle = false;

	if(mouseinitialized)
	{
		if(dinput)
		{
			if(g_pMouse)
			{
				if(dinput_acquired)
				{
					IDirectInputDevice_Unacquire(g_pMouse);
					dinput_acquired = false;
				};
			};
		}
		else
		{
			if(restore_spi)
				SystemParametersInfo(SPI_SETMOUSE, 0, originalmouseparms, 0);

			ClipCursor(nullptr);
			ReleaseCapture();
		};

		mouseactive = false;
	};
};

/*
===========
IN_ShowMouse
===========
*/
void CMouseWin::Show()
{
	if(!mouseshowtoggle)
	{
		ShowCursor(TRUE);
		mouseshowtoggle = 1;
	};
};

/*
===========
IN_HideMouse
===========
*/
void CMouseWin::Hide()
{
	if(mouseshowtoggle)
	{
		ShowCursor(FALSE);
		mouseshowtoggle = 0;
	};
};

/*
===========
IN_Accumulate
===========
*/
void CMouseWin::Accumulate()
{
	int mx, my;
	HDC hdc;

	if(mouseactive)
	{
		if(!dinput)
		{
			GetCursorPos(&current_pos);

			mx_accum += current_pos.x - window_center_x;
			my_accum += current_pos.y - window_center_y;

			// force the mouse to the center, so there's room to move
			SetCursorPos(window_center_x, window_center_y);
		};
	};
};

/*
===========
IN_MouseEvent
===========
*/
void CMouseWin::HandleEvent(int mstate)
{
	int i;

	if(mouseactive && !dinput)
	{
		// perform button actions
		for(i = 0; i < mouse_buttons; i++)
		{
			if((mstate & (1 << i)) && !(mouse_oldbuttonstate & (1 << i)))
			{
				Key_Event(K_MOUSE1 + i, true);
			};

			if(!(mstate & (1 << i)) && (mouse_oldbuttonstate & (1 << i)))
			{
				Key_Event(K_MOUSE1 + i, false);
			};
		};

		mouse_oldbuttonstate = mstate;
	};
};

/*
===========
IN_InitDInput
===========
*/
bool CMouseWin::InitDInput()
{
	HRESULT hr;
	DIPROPDWORD dipdw = {
		{
		sizeof(DIPROPDWORD),  // diph.dwSize
		sizeof(DIPROPHEADER), // diph.dwHeaderSize
		0,                    // diph.dwObj
		DIPH_DEVICE,          // diph.dwHow
		},
		DINPUT_BUFFERSIZE, // dwData
	};

	if(!hInstDI)
	{
		hInstDI = LoadLibrary("dinput.dll");

		if(hInstDI == nullptr)
		{
			Con_SafePrintf("Couldn't load dinput.dll\n");
			return false;
		};
	};

	if(!pDirectInputCreate)
	{
		pDirectInputCreate = (void *)GetProcAddress(hInstDI, "DirectInputCreateA");

		if(!pDirectInputCreate)
		{
			Con_SafePrintf("Couldn't get DI proc addr\n");
			return false;
		};
	};

	// register with DirectInput and get an IDirectInput to play with.
	hr = iDirectInputCreate(global_hInstance, DIRECTINPUT_VERSION, &g_pdi, nullptr);

	if(FAILED(hr))
		return false;

	// obtain an interface to the system mouse device.
	hr = IDirectInput_CreateDevice(g_pdi, &GUID_SysMouse, &g_pMouse, nullptr);

	if(FAILED(hr))
	{
		Con_SafePrintf("Couldn't open DI mouse device\n");
		return false;
	};

	// set the data format to "mouse format".
	hr = IDirectInputDevice_SetDataFormat(g_pMouse, &df);

	if(FAILED(hr))
	{
		Con_SafePrintf("Couldn't set DI mouse format\n");
		return false;
	};

	// set the cooperativity level.
	hr = IDirectInputDevice_SetCooperativeLevel(g_pMouse, mainwindow,
	                                            DISCL_EXCLUSIVE | DISCL_FOREGROUND);

	if(FAILED(hr))
	{
		Con_SafePrintf("Couldn't set DI coop level\n");
		return false;
	};

	// set the buffer size to DINPUT_BUFFERSIZE elements.
	// the buffer size is a DWORD property associated with the device
	hr = IDirectInputDevice_SetProperty(g_pMouse, DIPROP_BUFFERSIZE, &dipdw.diph);

	if(FAILED(hr))
	{
		Con_SafePrintf("Couldn't set DI buffersize\n");
		return false;
	};

	return true;
};