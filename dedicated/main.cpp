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
	
	pHost->Init();
	
	static int frame{0};
	
	while(frame < 1000)
	{
		++frame;
		pHost->Frame(0.1);
	};
	
	pHost->Shutdown();
	
	return EXIT_SUCCESS;
};