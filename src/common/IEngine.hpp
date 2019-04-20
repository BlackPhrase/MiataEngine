/// @file

#pragma once

#ifdef _WIN32
#	include <windows.h>
#endif

struct IEngine
{
	struct InitProps
	{
		struct PlatformProps
		{
#ifdef _WIN32
			HINSTANCE hInstance{};
#endif
		};
		
		PlatformProps PlatformSpecific{};
		
		bool mbDedicated{false};
	};
	
	///
	bool (*Init)(const InitProps &apInitProps);
	
	///
	void (*Frame)();
};

using fnGetEngine = IEngine *(*)();