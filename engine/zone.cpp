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

#include "quakedef.hpp"

/*
==============================================================================

						ZONE MEMORY ALLOCATION

There is never any space between memblocks, and there will never be two
contiguous free memblocks.

The rover can be left pointing at a non-empty block

The zone calls are pretty much only used for small strings and structures,
all big things are allocated on the hunk.
==============================================================================
*/

constexpr auto ZONEID = 0x1d4a11;
constexpr auto MINFRAGMENT = 64;

typedef struct memblock_s
{
	int size; // including the header and possibly tiny fragments
	int tag;  // a tag of 0 is a free block
	int id;   // should be ZONEID
	struct memblock_s *next, *prev;
	int pad; // pad to 64 bit boundary
} memblock_t;

typedef struct
{
	int size;             // total bytes malloced, including header
	memblock_t blocklist; // start / end cap for linked list
	memblock_t *rover;
} memzone_t;

memzone_t *mainzone{nullptr};

/*
========================
Z_Free
========================
*/
void Z_Free(void *ptr)
{
	//if(!ptr)
		//Sys_Error("Z_Free: NULL pointer");

	memblock_t *block = (memblock_t *)((byte *)ptr - sizeof(memblock_t));
	
	//if(block->id != ZONEID)
		//Sys_Error("Z_Free: freed a pointer without ZONEID");
	
	//if(block->tag == 0)
		//Sys_Error("Z_Free: freed a freed pointer");

	block->tag = 0; // mark as free

	memblock_t *other = block->prev;
	
	if(!other->tag)
	{
		// merge with previous free block
		other->size += block->size;
		other->next = block->next;
		other->next->prev = other;
		
		if(block == mainzone->rover)
			mainzone->rover = other;
		
		block = other;
	};

	other = block->next;
	
	if(!other->tag)
	{
		// merge the next free block onto the end
		block->size += other->size;
		block->next = other->next;
		block->next->prev = block;
		
		if(other == mainzone->rover)
			mainzone->rover = block;
	};
};

void *Z_TagMalloc(int size, int tag)
{
	int extra;
	memblock_t *start, *rover, *pnew, *base;

	//if(!tag)
		//Sys_Error("Z_TagMalloc: tried to use a 0 tag");

	//
	// scan through the block list looking for the first free block
	// of sufficient size
	//
	size += sizeof(memblock_t); // account for size of block header
	size += 4;                  // space for memory trash tester
	size = (size + 7) & ~7;     // align to 8-byte boundary

	base = rover = mainzone->rover;
	start = base->prev;

	do
	{
		if(rover == start) // scaned all the way around the list
			return NULL;
		
		if(rover->tag)
			base = rover = rover->next;
		else
			rover = rover->next;
	}
	while(base->tag || base->size < size);

	//
	// found a block big enough
	//
	extra = base->size - size;
	
	if(extra > MINFRAGMENT)
	{
		// there will be a free fragment after the allocated block
		pnew = (memblock_t *)((byte *)base + size);
		pnew->size = extra;
		pnew->tag = 0; // free block
		pnew->prev = base;
		pnew->id = ZONEID;
		pnew->next = base->next;
		pnew->next->prev = pnew;
		base->next = pnew;
		base->size = size;
	};

	base->tag = tag; // no longer a free block

	mainzone->rover = base->next; // next allocation will start looking here

	base->id = ZONEID;

	// marker for memory trash testing
	*(int *)((byte *)base + base->size - 4) = ZONEID;

	return (void *)((byte *)base + sizeof(memblock_t));
};

/*
========================
Z_Malloc
========================
*/
void *Z_Malloc(int size)
{
	//Z_CheckHeap(); // DEBUG
	
	void *buf = Z_TagMalloc(size, 1);
	
	//if(!buf)
		//Sys_Error("Z_Malloc: failed on allocation of %i bytes", size);
	
	Q_memset(buf, 0, size);

	return buf;
};