#pragma once

#include "ILogger.hpp"

//IEngineExport
struct IEngineInterface
{
	static constexpr auto Version{1};
	
	virtual int GetVersion() const = 0;
	
	virtual ILogger *GetLogger() const = 0;
};