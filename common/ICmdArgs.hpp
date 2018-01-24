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
/// @brief command arguments interface

#pragma once

struct ICmdArgs
{
	/// Will return an empty string, not a nullptr
	/// if arg > argc, so string operations are always safe
	virtual const char *GetByIndex(int id) const = 0;
	
	/// @return argument count
	virtual int GetCount() const = 0;
	
	///
	virtual const char *ToString() const = 0;
};