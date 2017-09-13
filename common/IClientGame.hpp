#pragma once

struct IClientGame
{
	///
	virtual bool Init() = 0;

	///
	virtual void Shutdown() = 0;

	///
	virtual void Frame() = 0;
};

using pfnGetClientGame = IClientGame *(*)();