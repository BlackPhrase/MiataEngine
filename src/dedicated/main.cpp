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
/// @brief dedicated mode engine launcher

#include <cstdlib>
#include "shared_lib.hpp"
#include "engine/IEngine.hpp"

int main(int argc, char **argv)
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
	
	host_parms.argc = argc;
	host_parms.argv = argv;
	
	host_parms.dedicated = true;
	
	//printf("Host_Init\n");
	pEngine->Init(&host_parms);
	
	// run one frame immediately for first heartbeat
	//pEngine->Frame();
	
	//
	// main loop
	//
	while(true)
		pEngine->Frame();
	
	pEngine->Shutdown();
	
	// return success of application
	return EXIT_SUCCESS;
};