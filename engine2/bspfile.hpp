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

/// @file
/// @brief bsp file format description

#pragma once

// upper design bounds

constexpr auto MAX_MAP_HULLS = 4;

constexpr auto MAX_MAP_MODELS = 256;
constexpr auto MAX_MAP_BRUSHES = 4096;
constexpr auto MAX_MAP_ENTITIES = 1024;
constexpr auto MAX_MAP_ENTSTRING = 65536;

#define MAX_MAP_PLANES 32767
#define MAX_MAP_NODES 32767     ///< because negative shorts are contents
#define MAX_MAP_CLIPNODES 32767
#define MAX_MAP_LEAFS 8192
#define MAX_MAP_VERTS 65535
#define MAX_MAP_FACES 65535
#define MAX_MAP_MARKSURFACES 65535
#define MAX_MAP_TEXINFO 4096
#define MAX_MAP_EDGES 256000
#define MAX_MAP_SURFEDGES 512000
#define MAX_MAP_TEXTURES 512

#define MAX_MAP_MIPTEX 0x200000
#define MAX_MAP_LIGHTING 0x100000
#define MAX_MAP_VISIBILITY 0x100000

constexpr auto MAX_MAP_PORTALS = 65536;

// key / value pair sizes

constexpr auto MAX_KEY = 32;
constexpr auto MAX_VALUE = 1024;

//=============================================================================

constexpr auto BSPVERSION = 29;
constexpr auto TOOLVERSION = 2;

struct lump_t
{
	int fileofs, filelen;
};

enum
{
	LUMP_ENTITIES = 0,
	LUMP_PLANES,
	LUMP_TEXTURES,
	LUMP_VERTEXES,
	LUMP_VISIBILITY,
	LUMP_NODES,
	LUMP_TEXINFO,
	LUMP_FACES,
	LUMP_LIGHTING,
	LUMP_CLIPNODES,
	LUMP_LEAFS,
	LUMP_MARKSURFACES,
	LUMP_EDGES,
	LUMP_SURFEDGES,
	LUMP_MODELS,

	HEADER_LUMPS
};

typedef struct
{
	vec3_t mins{}, maxs{};
	vec3_t origin{};
	int headnode[MAX_MAP_HULLS]{};
	int visleafs{0}; ///< not including the solid leaf 0
	int firstface{0}, numfaces{0};
} dmodel_t;

typedef struct
{
	int version{0};
	lump_t lumps[HEADER_LUMPS]{};
} dheader_t;

struct dmiptexlump_t
{
	int nummiptex{0};
	int dataofs[4]{}; ///< [nummiptex]
};

constexpr auto MIPLEVELS = 4;

typedef struct miptex_s
{
	char name[16];
	unsigned width, height;
	unsigned offsets[MIPLEVELS]; ///< four mip maps stored
} miptex_t;

struct dvertex_t
{
	vec3_t point;
};

/// 0-2 are axial planes
#define PLANE_X 0
#define PLANE_Y 1
#define PLANE_Z 2

/// 3-5 are non-axial planes snapped to the nearest
#define PLANE_ANYX 3
#define PLANE_ANYY 4
#define PLANE_ANYZ 5

struct dplane_t
{
	vec3_t normal{};
	float dist{0.0f};
	int type{0}; ///< PLANE_X - PLANE_ANYZ ?remove? trivial to regenerate
};

#define CONTENTS_EMPTY -1
#define CONTENTS_SOLID -2
#define CONTENTS_WATER -3
#define CONTENTS_SLIME -4
#define CONTENTS_LAVA -5
#define CONTENTS_SKY -6
#define CONTENTS_ORIGIN -7 ///< removed at csg time
#define CONTENTS_CLIP -8   ///< changed to contents_solid

#define CONTENTS_CURRENT_0 -9
#define CONTENTS_CURRENT_90 -10
#define CONTENTS_CURRENT_180 -11
#define CONTENTS_CURRENT_270 -12
#define CONTENTS_CURRENT_UP -13
#define CONTENTS_CURRENT_DOWN -14

// !!! if this is changed, it must be changed in asm_i386.h too !!!
typedef struct
{
	int planenum;
	short children[2]; ///< negative numbers are -(leafs+1), not nodes
	short mins[3];     ///< for sphere culling
	short maxs[3];
	unsigned short firstface;
	unsigned short numfaces; ///< counting both sides
} dnode_t;

typedef struct
{
	int planenum;
	short children[2]; ///< negative numbers are contents
} dclipnode_t;

typedef struct texinfo_s
{
	float vecs[2][4]; ///< [s/t][xyz offset]
	int miptex;
	int flags;
} texinfo_t;

#define TEX_SPECIAL 1 ///< sky or slime, no lightmap or 256 subdivision

/// note that edge 0 is never used, because negative edge nums are used for
/// counterclockwise use of the edge in a face
typedef struct
{
	unsigned short v[2]; ///< vertex numbers
} dedge_t;

#define MAXLIGHTMAPS 4

typedef struct
{
	short planenum;
	short side;

	int firstedge; ///< we must support > 64k edges
	
	short numedges;
	short texinfo;

	// lighting info
	byte styles[MAXLIGHTMAPS];
	int lightofs; ///< start of [numstyles * surfsize] samples
} dface_t;

enum
{
	AMBIENT_WATER = 0,
	AMBIENT_SKY,
	AMBIENT_SLIME,
	AMBIENT_LAVA,

	NUM_AMBIENTS ///< automatic ambient sounds
};

/// leaf 0 is the generic CONTENTS_SOLID leaf, used for all solid areas
/// all other leafs need visibility info
struct dleaf_t
{
	int contents;
	int visofs; ///< -1 = no visibility info

	short mins[3]; ///< for frustum culling
	short maxs[3];

	unsigned short firstmarksurface;
	unsigned short nummarksurfaces;

	byte ambient_level[NUM_AMBIENTS];
};

//============================================================================

#ifndef QUAKE_GAME

#define ANGLE_UP -1
#define ANGLE_DOWN -2

// the utilities get to be lazy and just use large static arrays

extern int nummodels;
extern dmodel_t dmodels[MAX_MAP_MODELS];

extern int visdatasize;
extern byte dvisdata[MAX_MAP_VISIBILITY];

extern int lightdatasize;
extern byte dlightdata[MAX_MAP_LIGHTING];

extern int texdatasize;
extern byte dtexdata[MAX_MAP_MIPTEX]; // (dmiptexlump_t)

extern int entdatasize;
extern char dentdata[MAX_MAP_ENTSTRING];

extern int numleafs;
extern dleaf_t dleafs[MAX_MAP_LEAFS];

extern int numplanes;
extern dplane_t dplanes[MAX_MAP_PLANES];

extern int numvertexes;
extern dvertex_t dvertexes[MAX_MAP_VERTS];

extern int numnodes;
extern dnode_t dnodes[MAX_MAP_NODES];

extern int numtexinfo;
extern texinfo_t texinfo[MAX_MAP_TEXINFO];

extern int numfaces;
extern dface_t dfaces[MAX_MAP_FACES];

extern int numclipnodes;
extern dclipnode_t dclipnodes[MAX_MAP_CLIPNODES];

extern int numedges;
extern dedge_t dedges[MAX_MAP_EDGES];

extern int nummarksurfaces;
extern unsigned short dmarksurfaces[MAX_MAP_MARKSURFACES];

extern int numsurfedges;
extern int dsurfedges[MAX_MAP_SURFEDGES];

void DecompressVis(byte *in, byte *decompressed);
int CompressVis(byte *vis, byte *dest);

void LoadBSPFile(const char *filename);
void WriteBSPFile(const char *filename);
void PrintBSPFileSizes();

//===============

typedef struct epair_s
{
	struct epair_s *next;
	char *key;
	char *value;
} epair_t;

typedef struct
{
	vec3_t origin;
	int firstbrush;
	int numbrushes;
	epair_t *epairs;
} entity_t;

extern int num_entities;
extern entity_t entities[MAX_MAP_ENTITIES];

void ParseEntities();
void UnparseEntities();

void SetKeyValue(entity_t *ent, const char *key, const char *value);

/// will return "" if not present
const char *ValueForKey(entity_t *ent, const char *key);

vec_t FloatForKey(entity_t *ent, const char *key);
void GetVectorForKey(entity_t *ent, const char *key, vec3_t vec);

epair_t *ParseEpair();

#endif // QUAKE_GAME