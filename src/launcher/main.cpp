/*
Copyright (C) 2017-2018 BlackPhrase

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
#include "shared_lib.hpp"
#include "engine/IEngine.hpp"

//#ifdef _WIN32
	//#include <windows.h>
	
	//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//#else
	int main(int argc, char **argv)
//#endif
{
	shiftutil::shared_lib EngineLib("engine");
	
	if(!EngineLib)
		return EXIT_FAILURE;
	
	auto fnGetEngine{EngineLib.getexport<pfnGetEngine>("GetEngine")};
	
	if(!fnGetEngine)
		return EXIT_FAILURE;
	
	auto *pEngine{fnGetEngine()};
	
	if(!pEngine)
		return EXIT_FAILURE;
	
	quakeparms_t host_parms{};
	
	// TODO
	//host_parms.argc = 0;
	//host_parms.argv = nullptr;
	
	pEngine->Init(&host_parms);
	
	while(true)
		pEngine->Frame();
	
	pEngine->Shutdown();
	
	return EXIT_SUCCESS;
};