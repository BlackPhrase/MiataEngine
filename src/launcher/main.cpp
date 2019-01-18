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

#include <cstdlib>

#include "IEngine.hpp"

#ifdef _WIN32
HINSTANCE ghInstance{0};
#endif

int main(int argc, char **argv)
{
	IEngine *pEngine{GetEngine()};
	
	IEngine::InitProps InitProps{};
	
#ifdef _WIN32
	InitProps.PlatformSpecific.hInstance = ghInstance;
#endif
	
	if(!pEngine->Init(InitProps))
		return EXIT_FAILURE;
	
    // main window message loop
	while(true)
		pEngine->Frame();

    // return success of application
    return EXIT_SUCCESS;
};

/*
==================
WinMain
==================
*/
#ifdef _WIN32
HWND hwnd_dialog{};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// previous instances do not exist in Win32
    if(hPrevInstance)
        return EXIT_FAILURE;
	
	ghInstance = hInstance;
	
	hwnd_dialog = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), nullptr, nullptr);

	if (hwnd_dialog)
	{
		RECT rect{};
		
		if (GetWindowRect (hwnd_dialog, &rect))
		{
			if (rect.left > (rect.top * 2))
			{
				SetWindowPos (hwnd_dialog, 0,
					(rect.left / 2) - ((rect.right - rect.left) / 2),
					rect.top, 0, 0,
					SWP_NOZORDER | SWP_NOSIZE);
			};
		};

		ShowWindow (hwnd_dialog, SW_SHOWDEFAULT);
		UpdateWindow (hwnd_dialog);
		SetForegroundWindow (hwnd_dialog);
	};
	
	return main(__argc, __argv);
};
#endif