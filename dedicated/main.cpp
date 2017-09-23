#include <cstdlib>
#include "shared_lib.hpp"
#include "IHost.hpp"

int main(int argc, char **argv)
{
	shiftutil::shared_lib EngineLib("engine");
	
	if(!EngineLib)
		return EXIT_FAILURE;
	
	pfnGetHost fnGetHost{EngineLib.getexport<pfnGetHost>("GetHost")};
	
	if(!fnGetHost)
		return EXIT_FAILURE;
	
	IHost *pHost{fnGetHost()};
	
	if(!pHost)
		return EXIT_FAILURE;
	
	quakeparms_t host_parms{};
	
	host_parms.argc = argc;
	host_parms.argv = argv;
	
	host_parms.dedicated = true;
	
	//printf("Host_Init\n");
	pHost->Init(&host_parms);
	
	// run one frame immediately for first heartbeat
	//pHost->Frame();
	
	//
	// main loop
	//
	while(true)
		pHost->Frame();
	
	pHost->Shutdown();
	
	// return success of application
	return EXIT_SUCCESS;
};