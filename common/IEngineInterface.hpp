#pragma once

#include "ILogger.hpp"

//IEngineExport
struct IEngineInterface
{
	virtual ILogger *GetLogger() const = 0;
};