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

#include "quakedef.hpp"
#include "WADArchive.hpp"

CWADManager::CWADManager() = default;
CWADManager::~CWADManager() = default;

/*
====================
W_LoadWadFile
====================
*/
void CWADManager::LoadWadFile(TWADData *pData, const char *filename)
{
	pData->wad_base = COM_LoadHunkFile(filename);
	
	//if(!pData->wad_base)
		//Sys_Error("W_LoadWadFile: couldn't load %s", filename);

	wadinfo_t *header{(wadinfo_t *)pData->wad_base};

	//if(header->identification[0] != 'W' || header->identification[1] != 'A' || header->identification[2] != 'D' || header->identification[3] != '2')
		//Sys_Error("Wad file %s doesn't have WAD2 id\n", filename);

	pData->wad_numlumps = LittleLong(header->numlumps);
	
	int infotableofs{LittleLong(header->infotableofs)};
	
	pData->wad_lumps = (lumpinfo_t *)(pData->wad_base + infotableofs);

	lumpinfo_t *lump_p{nullptr};
	
	for(unsigned i = 0, lump_p = pData->wad_lumps; i < pData->wad_numlumps; i++, lump_p++)
	{
		lump_p->filepos = LittleLong(lump_p->filepos);
		lump_p->size = LittleLong(lump_p->size);
		
		W_CleanupName(lump_p->name, lump_p->name);
		
		if(lump_p->type == TYP_QPIC)
			SwapPic((qpic_t *)(pData->wad_base + lump_p->filepos));
	};
};