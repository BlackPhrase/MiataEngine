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
/// @brief game module interface

#pragma once

#include "IEngineInterface.hpp"

struct IGame
{
	/// The init function will only be called when a game starts,
	/// not each time a level is loaded. Persistant data for clients
	/// and the server can be allocated in init
	virtual bool Init() = 0;
	
	///
	virtual void Shutdown() = 0;
	
	///
	virtual void Frame() = 0;
};

using pfnGetGame = IGame *(*)(IEngineInterface *apEngineInterface);