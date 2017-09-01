#pragma once

struct IMenu
{
	///
	virtual bool Init() = 0;
	
	///
	virtual void Shutdown() = 0;
	
	///
	virtual void Frame() = 0;
};

using pfnGetMenu = IMenu *(*)();