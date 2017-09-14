/*
Copyright (C) 1996-1997 Id Software, Inc.
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

#pragma once

#include "commontypes.hpp"

class CSizeBuffer final
{
public:
	CSizeBuffer(const char *name);
	CSizeBuffer();
	~CSizeBuffer();
	
	void Clear();
	
	void *GetSpace(int length);
	
	void Write(const void *data, int length);
	void Print(const char *data); // strcats onto the sizebuf
//private:
	const char *name{""};
	
	byte *data{nullptr};
	
	int maxsize{0};
	int cursize{0};
	
	bool allowoverflow{false}; ///< if false, do a Sys_Error
	bool overflowed{false}; ///< set to true if the buffer size failed
};