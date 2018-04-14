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
/// @brief video driver defs

#pragma once

constexpr auto VID_CBITS = 6;
constexpr auto VID_GRADES = (1 << VID_CBITS);

/// a pixel can be one, two, or four bytes
typedef byte pixel_t;

struct vrect_t
{
	int x, y;
	int width, height;
	
	vrect_t *pnext;
};

typedef struct
{
	pixel_t *buffer;            // invisible buffer
	pixel_t *colormap;          // 256 * VID_GRADES size
	unsigned short *colormap16; // 256 * VID_GRADES size
	int fullbright;             // index of first fullbright color
	unsigned rowbytes;          // may be > width if displayed in a window
	
	unsigned width;
	unsigned height;
	
	float aspect; // width / height -- < 0 is taller than wide
	int numpages;
	int recalc_refdef; // if true, recalc vid-based stuff
	
	pixel_t *conbuffer;
	int conrowbytes;
	
	unsigned conwidth;
	unsigned conheight;
	
	int maxwarpwidth;
	int maxwarpheight;
	
	pixel_t *direct; // direct drawing to framebuffer, if not NULL
} viddef_t;

extern viddef_t vid; ///< global video state

// Video module initialization etc

/// flushes the given rectangles from the view buffer to the screen
//void VID_Update(vrect_t *rects);

/// sets the mode; only used by the Quake engine for resetting to mode 0 (the
/// base mode) on memory allocation failures
//int VID_SetMode(int modenum, byte *palette);

#ifdef _WIN32
	/// called only on Win32, when pause happens, so the mouse can be released
	//void VID_HandlePause(bool pause);
#endif

void VID_MenuInit();
void VID_MenuDraw();
const char *VID_MenuKey(int);