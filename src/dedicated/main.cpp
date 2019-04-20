/*
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
/// @brief dedicated mode engine launcher

#include <cstdlib>
#include "konbini/shared_lib.hpp"
#include "engine/IEngine.h"

int main(int argc, char **argv)
{
	konbini::shared_lib EngineLib("engine");
	
	if(!EngineLib)
		return EXIT_FAILURE;
	
	auto pfnGetEngine{EngineLib.getexportfunc<fnGetEngine>("GetEngine")};
	
	if(!pfnGetEngine)
		return EXIT_FAILURE;
	
	auto *pEngine{pfnGetEngine()};
	
	if(!pEngine)
		return EXIT_FAILURE;
	
	IEngine::InitProps InitProps{}; //quakeparms_t host_parms{};
	
	// TODO
	//InitProps.argc = argc;
	//InitProps.argv = argv;
	
	InitProps.mbDedicated = true;
	
	//printf("Host_Init\n");
	if(!pEngine->Init(InitProps))
		return EXIT_FAILURE;
	
	// run one frame immediately for first heartbeat
	//pEngine->Frame();
	
	//
	// main loop
	//
	while(true)
		pEngine->Frame();
	
	//pEngine->Shutdown();
	
	// return success of application
	return EXIT_SUCCESS;
};