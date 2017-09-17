#pragma once

#include "IEngineInterface.hpp"

struct IVideo
{
	/// Called at startup to set up translation tables, takes 256 8 bit RGB values
	/// the palette data will go away after the call, so it must be copied off if
	/// the video driver will need it again
	virtual bool Init() = 0;
	
	/// Called at shutdown
	virtual void Shutdown() = 0;
	
	///
	//virtual void Frame() = 0;
	
	///
	virtual void CheckChanges() = 0;
};

using pfnGetVideo = IVideo *(*)(IEngineInterface *apEngineInterface);