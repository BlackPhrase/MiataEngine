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
/// @brief host interface

#pragma once

//using pfnHost_Init = void (*)(quakeparms_t *parms);
//using pfnHost_Shutdown = void (*)();

//using pfnHost_Frame = void (*)(double frametime);

//=============================================================================

/// the host system specifies the base of the directory tree, the
/// command line parms passed to the program, and the amount of memory
/// available for the program to use

struct quakeparms_t
{
	char *basedir{nullptr};
	char *cachedir{nullptr}; ///< For development over ISDN lines
	
	int argc{0};
	char **argv{nullptr};
	
	void *membase{nullptr};
	int memsize{0};
	
	bool dedicated{false}; ///< dedicated server mode
};

struct IHost
{
	virtual bool Init(quakeparms_t *parms) = 0;
	virtual void Shutdown() = 0;

	virtual void Frame() = 0;
};

using pfnGetHost = IHost *(*)();