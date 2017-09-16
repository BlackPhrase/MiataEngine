/*
Copyright (C) 2017 Sh1ft0x0EF

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
/// @brief sound module interface

#pragma once

#include "IEngineInterface.hpp"

struct ISound
{
	///
	virtual bool Init() = 0;

	///
	virtual void Shutdown() = 0;

	///
	virtual void Frame() = 0;
};

using pfnGetSound = ISound *(*)(IEngineInterface *apEngineInterface);