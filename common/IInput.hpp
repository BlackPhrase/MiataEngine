#pragma once

struct IInput
{
	///
	virtual bool Init() = 0;
	
	///
	virtual void Shutdown() = 0;
	
	///
	virtual void Frame() = 0;
};

using pfnGetInput = IInput *(*)();