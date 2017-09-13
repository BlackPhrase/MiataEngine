#pragma once

struct ISound
{
	///
	virtual bool Init() = 0;

	///
	virtual void Shutdown() = 0;

	///
	virtual void Frame() = 0;
};

using pfnGetSound = ISound *(*)();