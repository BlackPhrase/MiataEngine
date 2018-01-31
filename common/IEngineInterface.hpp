/*
Copyright (C) 2017-2018 BlackPhrase

This program is free software: you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation, either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/// @file
/// @brief engine interface

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