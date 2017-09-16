#pragma once

#include "IEngineInterface.hpp"

struct IVideo
{
	virtual bool Init() = 0;
	
	///
	virtual void Shutdown() = 0;
	
	///
	//virtual void Frame() = 0;
	
	///
	virtual void CheckChanges() = 0;
};

using pfnGetVideo = IVideo *(*)(IEngineInterface *apEngineInterface);