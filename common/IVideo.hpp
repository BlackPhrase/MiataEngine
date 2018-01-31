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
/// @brief video interface

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