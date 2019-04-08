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
	virtual bool Init(const InitProps &apInitProps) = 0;
	
	///
	virtual void Frame() = 0;
};

using fnGetEngine = IEngine *(*)();