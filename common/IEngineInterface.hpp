#pragma once

#include "ILogger.hpp"
#include "ICmdBuffer.hpp"
#include "ICvarList.hpp"
#include "ICvarController.hpp"
#include "ICmdList.hpp"

//IEngineExport
struct IEngineInterface
{
	/// Engine interface version number
	static constexpr auto Version{1};
	
	/// @return version of the engine interface
	virtual int GetVersion() const = 0;
	
	/// @return pointer to the logger component
	virtual ILogger *GetLogger() const = 0;
	
	/// @return pointer to the command buffer
	virtual ICmdBuffer *GetCmdBuffer() const = 0;
	
	/// @return pointer to the console variable list
	virtual ICvarList *GetCvarList() const = 0;
	
	/// @return pointer to the console variable controller
	virtual ICvarController *GetCvarController() const = 0;
	
	/// @return pointer to the command list
	virtual ICmdList *GetCmdList() const = 0;
};