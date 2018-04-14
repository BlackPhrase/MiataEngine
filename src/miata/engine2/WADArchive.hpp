/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

/// @file

#pragma once

#include "commontypes.hpp"

struct lumpinfo_t
{
	int32_t filepos{0};
	int32_t disksize{0};
	int32_t size{0}; ///< uncompressed
	
	char type{0};
	char compression{0};
	char pad1{0}, pad2{0};
	char name[16]{}; ///< must be null terminated
};

struct TWADData
{
	byte *wad_base{nullptr};
	lumpinfo_t *wad_lumps{nullptr};
	
	int wad_numlumps{0};
};

extern TWADData gWADDAta;

class CWADManager final
{
public:
	CWADManager();
	~CWADManager();
	
	void LoadWadFile(const char *filename);
	
	lumpinfo_t *GetLumpinfo(TWADData *pData, const char *name);
private:
};