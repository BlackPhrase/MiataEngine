#include <cstdlib>
#include "shared_lib.hpp"
#include "IHost.hpp"

#ifdef _WIN32
	#include <windows.h>
	
	int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
	int main(int argc, char **argv)
#endif
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
	
	// TODO
	host_parms.argc = 0;
	host_parms.argv = nullptr;
	
	pHost->Init(&host_parms);
	
	static int frame{0};
	
	while(frame < 1000)
	{
		++frame;
		pHost->Frame(0.1);
	};
	
	pHost->Shutdown();
	
	return EXIT_SUCCESS;
};