#pragma once

//using pfnHost_Init = void (*)(quakeparms_t *parms);
//using pfnHost_Shutdown = void (*)();

//using pfnHost_Frame = void (*)(double frametime);

////////////////////////

struct IHost
{
	virtual bool Init(/*quakeparms_t *parms*/) = 0;
	virtual void Shutdown() = 0;

	virtual void Frame(double frametime) = 0;
};

using pfnGetHost = IHost *(*)();