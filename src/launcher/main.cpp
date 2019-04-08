/*
Copyright (C) 1996-1997 Id Software, Inc.
Copyright (C) 2017-2019 BlackPhrase

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

/// @file
/// @brief client mode engine launcher

#include <cstdlib>

#include "IEngine.hpp" // TODO: engine/IEngine.hpp

#include "konbini/shared_lib.hpp"

#ifdef _WIN32
#include <windows.h>

HINSTANCE ghInstance{0};
#endif

#ifndef MIATA_SINGLE_BINARY
konbini::shared_lib *gpEngineLib{nullptr};

void UnloadEngineModule()
{
	// TODO
};

IEngine *GetEngine()
{
	static konbini::shared_lib EngineLib("engine");
	
	if(!EngineLib)
		return nullptr;
	
	gpEngineLib = &EngineLib;
	
	auto pfnGetEngine{EngineLib.getexportfunc<fnGetEngine>("GetEngine")};
	
	if(!pfnGetEngine)
		return nullptr;
	
	return pfnGetEngine();
};
#endif // MIATA_SINGLE_BINARY

int main(int argc, char **argv)
{
	IEngine *pEngine{GetEngine()};
	
	if(!pEngine)
		return EXIT_FAILURE;
	
	IEngine::InitProps InitProps{}; // quakeparms_t host_parms{};
	
#ifdef _WIN32
	InitProps.PlatformSpecific.hInstance = ghInstance;
#endif
	
	// TODO
	//InitProps.argc = 0;
	//InitProps.argv = nullptr;

	if(!pEngine->Init(InitProps))
		return EXIT_FAILURE;
	
    // main window message loop
	while(true)
		pEngine->Frame();

	//pEngine->Shutdown();

    // return success of application
    return EXIT_SUCCESS;
};

/*
==================
WinMain
==================
*/
#ifdef _WIN32

#include "resource.h"

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